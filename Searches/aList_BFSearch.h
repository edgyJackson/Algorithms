#ifndef ALIST_BFSEARCH_H
#define ALIST_BFSEARCH_H
#include<iostream>
#include<queue>
#include<vector>
#include <ctype.h>
using namespace std;

void aList_BFS()
{
	//this is our char array with our edges to insert into the corrosponding row
int edges =21;
char edgeList[21][2]= {
    {'A','B'},
    {'A','D'},
    {'B','C'},
    {'C','M'},
    {'C','N'},
    {'D','E'},
    {'D','F'},
    {'D','G'},
    {'D','N'},
    {'E','F'},
    {'F','H'},
    {'G','H'},
    {'G','J'},
    {'H','I'},
    {'H','K'},
    {'I','J'},
    {'J','K'},
    {'J','N'},
    {'K','L'},
    {'L','M'},
    {'M','N'}

};

vector<int>nodes[1000];
cout<<"setting up graph via adj list"<<endl;

    for(int i=0;i<edges;i++){
        nodes[int(edgeList[i][0])].push_back(int(edgeList[i][1]));
        nodes[int(edgeList[i][1])].push_back(int(edgeList[i][0]));
    }

    cout<<endl;
 // these nodes will be derived from our Adj list, converted ints to char to display letters
for(int i=0;i<1000;i++)
{
    if(!nodes[i].empty())
    {	cout<<"[ "<<char(i)<<" ]"<<"-->";
        for(vector<int>::iterator it=nodes[i].begin();
                it!=nodes[i].end();++it)
        {
        cout<<char(*it)<<"-->";
        }
        cout<<"NULL"<<endl;
    }
}





queue<int> que;
//queue to keep track of next node to visit
bool visited[1000] = {false};
// mark all the vertices as not visited

char start;
cout<<"\nEnter the starting node"<<endl;
cin>>start;
start = toupper(start);
//start with the starting node and insert it into the queue
que.push(start);
//mark the node as visited so we don't count it twice
visited[int(start)]=true;


    cout<<"\nBFS Traversal\n";

while(!que.empty())
{
        //Dequeue a vertex from que and print it
        int front = que.front();
        cout<<char(front)<<" ";
        que.pop();
// get all adjacent vertices of the dequeued list
     // If an neighboring vertex has not been visited,
       //then mark it as visited
        // and add it to the queue
    for(vector<int>::iterator it=nodes[front].begin();
        it!=nodes[front].end();++it)
        {
            if(visited[*it]==false)
            {
                visited[*it]=true;
                que.push(*it);
            }
        }
}

    cout<<endl;

}

#endif // ALIST_BFSEARCH_H
