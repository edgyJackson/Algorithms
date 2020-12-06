#include <iostream>
#include <sstream>
#include <string>

using namespace std;
 
 
 void inorder_orginal(int s[][5],int l,int h)
 {
 	if(s[l][h]!=0)
 	{
 		cout<<"(";
	 	inorder_orginal(s,l,s[l][h]);
	 }

	if(s[l][h]==0) // l == h 
	{
        cout<<"A"<<h;
        
	}

    if(s[l][h]!=0)
    {
    
    inorder_orginal(s,s[l][h]+1,h);

    cout<<")";
    }
 }
 
 std::string inorder_fixed(int s[][5],int l,int h)
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

 
 
int main()
{
    int n=5,i;
    int p[]={5,4,6,2,7};
    int m[5][5]={0};
    int s[5][5]={0};
    
    int j,q;
 
    for (int d=1; d<n-1; d++)
    {
        for (int i=1; i<n-d; i++)
        {
            j = i+d;
            m[i][j] = 32767;  
 
            for (int k=i; k<=j-1; k++)
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
 
    cout<<m[1][n-1]<<" ";
    
    cout << inorder_fixed(s,1,4);
	
    return 0;
}
