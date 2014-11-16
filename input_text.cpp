/*
 * input_text.cpp
 *
 *  Created on: Jul 15, 2011
 *      Author: arun
 */
#include "input_text.h"
#include "levenshtein.h"
#include <queue>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <math.h>


using namespace std;

int inputFile::thread_indicator = 0;
unsigned long inputFile::m_bytes = 0;
char * inputFile::thread_file  = NULL;


string common_words_[15] = {"a","of","the","and","is","in","this","for","what","how","why","if","else","while","to"};

inline bool isCommon(string& word){

	//vector<string>::iterator itr ;
	//printf << "h : " << word << endl;
/*
	for(int i = 0 ; i < 15 ; i++){
		if(word == common_words_[i]){
			return true;
		}
	}
	return false;
*/

	if(word.length() <= 4)
		return true;
	else
		return false;

}

extern "C" void * thread_proc(void * arg){
	static_cast<inputFile *>(arg)->thread_func();
	return NULL;
}

inline void line_proc(char * line, int len, vector<string> & phrase){

	// len is upto '\n' i.e '\n' is not counted

	vector<string>::iterator s_vec = phrase.begin(); // contains tokenized phrase
	int i = 0;

	// gotta maintain number of common and uncommon words in the string to find
	// out the percentage relevancy
	len += 1;
	int common = 0;
	int uncommon = 0;

	vector<string> question ;  // will contain tokenized question line
	vector<string>::iterator q_itr;
	vector<string>::iterator ph_itr;

	while(i <= len){
		char * start = line;
		while((i < len) && (*line != ' ')){
			line++;
			i++;
		}
		line++;
		if(i <= len){
			string s(start,(int)(line - 1  - start));
			//printf("%s :: %d\n",s.c_str(),s.length());
			question.push_back(s);
			if(! isCommon(s))
				uncommon++;
			else
				common++;
		}

		i++;
	} // end main while loop

	// Percentage upto 2 decimal places

	float uncommon_perc = floorf(((float)90/uncommon)*100)/100;
	float common_perc = floorf(((float)10/common)*100)/100;
	float relevency = 0;

	// Calculate relevency


	for(ph_itr = phrase.begin() ; ph_itr != phrase.end() ; ph_itr++){

		for(q_itr = question.begin() ; q_itr != question.end() ; q_itr++){
			if(*ph_itr == *q_itr){

				if(isCommon(*ph_itr)){
					relevency += common_perc;
				}else{
						relevency += uncommon_perc;
				}
			}else{
				int dist = liv_distance(*ph_itr,*q_itr);

				if(dist <= floor(log(pow((long)ph_itr->length(),3))/2.303)){
					relevency += ((uncommon_perc) - 2 * dist);
					//printf("%s :: %s :: %d\n",ph_itr->c_str(),q_itr->c_str(),dist);
				}
			}

		}//end for

	}//end for


	printf("Line :: \n");
	for (int j = 0; j < question.size() ; j++){
		printf("%s ",question[j].c_str());
	}
	printf(" ::  %f\n",relevency);

}

void * inputFile::thread_func(){

	pthread_detach(pthread_self());

	pthread_mutex_lock(&(trd.mutex));

	while(inputFile::thread_indicator == 0 ){
		pthread_cond_wait(&(trd.cond),&(trd.mutex));
	}

	if(inputFile::thread_indicator == 1){   // Start data processing

		// start processing bottom half of the mapped file and insert data into &dat_str->thread_proc_data
		pthread_mutex_unlock(&(trd.mutex));
		printf ("Got conditioned :: %d\n",inputFile::thread_indicator);

		//int bytes = dat_str->file_size - dat_str->start_bytes;
		printf(" Bytes :: %d\n", inputFile::m_bytes);

		char *start;
		unsigned long i = 0;
		while(i <= inputFile::m_bytes){
			start = inputFile::thread_file;
			while(*(inputFile::thread_file) != '\n'){
				i++;
				//printf("%c\n",*(inputFile::thread_file));
				inputFile::thread_file++;

			}
			line_proc(start,static_cast<int>(inputFile::thread_file - start - 1),trd.phrase);
			inputFile::thread_file++;
			i++;
		}

	}else{
		printf("Condition variable ERROR :: %d\n",inputFile::thread_indicator);
		pthread_mutex_unlock(&(trd.mutex));
	}
	return NULL;
}

void inputFile::load_file(const char * file_name){

		fd = open(file_name,O_RDONLY);
		if(fd == -1){
			cout << "Error while opening input file" << endl;
			exit(1);
		}

		struct stat sb;
		fstat(fd,&sb);
		file_size = sb.st_size;
		file = static_cast<char *>(mmap (0,file_size,PROT_READ,MAP_SHARED,fd,0));

		if(file == MAP_FAILED){
			cout << "Error while memory mapping" << endl;
			exit(1);
		}

}


void inputFile::start_processing(common& com , searchable& phrases){
	//pthread_t thread1;
	pthread_mutex_init(&trd.mutex,NULL);
	pthread_cond_init(&trd.cond,NULL);
	pthread_create(&thread1,NULL,thread_proc,this);

	//trd.fil_obj = this;
	// Set the char pointer for the thread

	char * temp = file;
	temp += file_size/2;
	//cout.setf(std::ios::unitbuf);
	int bytes = temp - file;

	while(*temp != '\n'){
		bytes++;
		temp++;
		//cout << "unlimited" << endl;
	}

	if(*temp == '\n'){

	                temp++;
	                inputFile::thread_file = temp;
	                trd.phrase = phrases.text_to_search[0];
	                inputFile::m_bytes = file_size - bytes + 1;

	                cout << "Res :: " << m_bytes << endl;
	                pthread_mutex_lock(&(trd.mutex));
	                inputFile::thread_indicator = 1;
	                pthread_cond_signal(&(trd.cond));
	                pthread_mutex_unlock(&(trd.mutex));

	                // Now process half of the file and put the results in record vector

	                temp--; // this thread will process upto '\n'

	                int i = 0;

	                char *start;
	                int status;
	                //status = pthread_join(thread1,NULL);
	                //cout << "thread ended :: " << status << endl;
	                while(1){
	                	;
	                }

/*
	                while(i <= bytes){
	                        start = file;
	                        while(*file != '\n'){
	                                //printf << *file << endl;
	                                i++;
	                                file++;
	                        }
	                        //printf << i << endl;

	                        line_proc(start,static_cast<int>(file - start - 1),phrases.text_to_search[0]);
	                        file++;
	                        i++;
	                }
*/

	        }
	}
