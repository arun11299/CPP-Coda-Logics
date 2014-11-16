/*
 * input_text.h
 *
 *  Created on: Jul 15, 2011
 *      Author: arun
 */

#ifndef INPUT_TEXT_H_
#define INPUT_TEXT_H_

#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
extern "C"{
#include <pthread.h>
}
//#include "common_words.h"
#include "to_search.h"

struct thread_related_data{
	vector < pair<string,int> > thread_proc_data;
	vector <string> phrase;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};


class inputFile{
public:
	inputFile(){

			main_proc_data.reserve(5000);
			//trd.thread_proc_data.reserve(2500);

			//trd.start_bytes = 0;
			//cout << "Going to start thread" << endl;

	}

	void load_file(const char *);

	void start_processing(common& , searchable& );

	//void line_proc(char *, int , vector< vector<string> >&);

	void * thread_func();

	void unload_file(){

		if (munmap(file, file_size) == -1) {
			//cout << "Error un-mmapping the file" << endl;
		}
		close(fd);
	}

	~inputFile(){}
private:
	inputFile(const inputFile& cc){}
	char * file;       // Points  to the memory mapped region
	static char * thread_file; // points to memory mapped region accesible by thread
	int fd ; // File descriptor
	int file_size;
	vector< pair<string,int> > main_proc_data; // will store the record for matched entries i.e if relevency is greater than 0.0%
	struct thread_related_data trd;
	static int thread_indicator;
	static unsigned long m_bytes;
	pthread_t thread1;
};



#endif /* INPUT_TEXT_H_ */
