#ifndef MEMOIZATIONMCM_H
#define MEMOIZATIONMCM_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "dynamicMCM.h"

//https://stackoverflow.com/questions/8079889/memoized-version-of-matrix-chain-multiplication
//from Introduction to Algorithms by cormen
//psuedo code for memoization version

/*
MEMOIZED-MATRIX-CHAIN(p)

 n  length[p] - 1
for i = 1 to n
      do for j = i to n
             do m[i, j]  = infinity
return LOOKUP-CHAIN(p, 1, n)

LOOKUP-CHAIN(p, i, j)

 if m[i,j] < infinity
    then return m[i, j]
 if i = j
   then m[i, j]  0
   else for k =  i to j - 1
             do q = LOOKUP-CHAIN(p, i, k) +
                     LOOKUP-CHAIN(p, k + 1, j) +
                     p(i - 1)* p(k) *p(j)
               if q < m[i, j]
 return m[i, j]

*/

//https://stackoverflow.com/questions/42048382/memoized-matrix-chain-multiplication-in-java



int lookUp_Chain(std::vector<int> &p,std::vector<std::vector<int> > &m, std::vector<std::vector<int> > &s, int i, int j) {

    int q;
    if(m[i][j] != -1) {
        return m[i][j];
    }
    if(i==j) {
        m[i][j] = 0;
    } else {

		
			
			for (int k = i; k < j; k++) {
				q = lookUp_Chain(p, m, s, i, k) + lookUp_Chain(p, m, s, k + 1, j) + p[i - 1] * p[k] * p[j];
				if (m[i][j] == -1) {
					m[i][j] = q;
					s[i][j] = k;
				}
				else if (q < m[i][j]) {
					m[i][j] = q;
					s[i][j] = k;

				}
			}

		
   }

    return m[i][j];

}

void memMCM (std::vector<int> &p){
    unsigned int n = p.size();
    std::vector<std::vector<int> > m (n, std::vector< int> (n));
	std::vector<std::vector<int> > s (n, std::vector< int>(n));
	
    for (unsigned int i=1;i<=n;i++) {
            for (unsigned int j=i;j<n;j++) {
                m[i][j] = -1;
            }
        }

    std::cout << lookUp_Chain(p,m,s,1,n-1) << " ";
	std::cout << inorder_fixed(s, 1, n - 1) << "\n\n";
}







#endif // MEMOIZATIONMCM_H
