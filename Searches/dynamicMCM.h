#ifndef DYNAMICMCM_H
#define DYNAMICMCM_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//Prints the matrices in order and parenthesized based on the values in the s matrix
 std::string inorder_fixed(std::vector< std::vector<int> > &s,int l,int h)
 {
    if(s[l][h] != 0)
    {
         std::ostringstream os;

         os << "(";
         os << inorder_fixed(s,l,s[l][h]);

         os << " x ";

         os << inorder_fixed(s,s[l][h]+1,h);

         os << ")";

         return os.str();
     }
    else
    {
        std::ostringstream os;
        os << "A"<<h;

        return os.str();
    }

 }

 void dPMCM (std::vector<int> &p, int n){
     std::vector<std::vector<int> > m (n, std::vector< int> (n));
     std::vector<std::vector<int> > s (n, std::vector< int> (n));
 int j,q;
 for ( int d=1; d<n-1; d++)
 {
     for ( int i=1; i<n-d; i++)
     {
         j = i+d;
         m[i][j] = 32767;

         for ( int k=i; k<j; k++)
         {
             q = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];
             if (q < m[i][j])
             {
                 m[i][j] = q;
         s[i][j] =k;
             }
         }
     }
 }

 std::cout<<m[1][n-1]<<" ";

 std::cout << inorder_fixed(s,1,n-1) << "\n\n";


}

#endif // DYNAMICMCM_H
