/*
 * levenshtein.h
 *
 *  Created on: Jul 14, 2011
 *      Author: arun
 */

#ifndef LEVENSHTEIN_H_
#define LEVENSHTEIN_H_

#include <vector>
#include <string>
#include <algorithm>

using namespace std;


int liv_distance(const string & source , const string & target){
	const int n = source.length();
	  const int m = target.length();
	  if (n == 0) {
	    return m;
	  }
	  if (m == 0) {
	    return n;
	  }

	  typedef std::vector< std::vector<int> > Tmatrix;

	  Tmatrix matrix(n+1);

	  for (int i = 0; i <= n; i++) {
	    matrix[i].resize(m+1);
	  }

	  for (int i = 0; i <= n; i++) {
	    matrix[i][0]=i;
	  }

	  for (int j = 0; j <= m; j++) {
	    matrix[0][j]=j;
	  }

	  for (int i = 1; i <= n; i++) {

	    const char s_i = source[i-1];

	    for (int j = 1; j <= m; j++) {

	      const char t_j = target[j-1];

	      int cost;
	      if (s_i == t_j) {
	        cost = 0;
	      }
	      else {
	        cost = 1;
	      }

	      const int above = matrix[i-1][j];
	      const int left = matrix[i][j-1];
	      const int diag = matrix[i-1][j-1];
	      const int cell = min( above + 1, min(left + 1, diag + cost));


	      matrix[i][j]=cell;
	    }
	  }


	  return matrix[n][m];

} // End Function

#endif /* LEVENSHTEIN_H_ */
