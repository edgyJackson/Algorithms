#include <iostream>
#include <sstream>
#include <string>
#include "dynamicMCM.h"
#include "aList_BFSearch.h"
#include "aMatrix_BFSearch.h"
#include "memoizationMCM.h"

using namespace std;

int main()
{



    cout << "------------------Part 1-2------------------------------------" << endl;

    cout << "Implement the DP version of MCM algorithm.\n\n\n";
    cout << "Implement the memoization version of MCM algorithm, using -1 for infinity.\n\n\n";







    cout << "------------------Part 3-4------------------------------------" << endl;
    cout << "Output for DP version of MCM algorithm for p being < 30,4,8,5,10,21,32,10,14,15,11,12,3,6,14,15,6,39,10,30>\n\n";

	vector<int> lab3p = { 30,4,8,5,10,21,32,10,14,15,11,12,3,6,14,15,6,39,10,30};
    dPMCM(lab3p,20);



    int i=0,n=0, input=0;
    cout << "Try it!! Enter size of MatrixChain";
    cin >> n;
    cout << "Enter values of MatrixChain : \n\n";
    std::vector<int> p (n);
    while (i < n) {
        cin >> input;
        p[i] = input;
        i++;
    }
    dPMCM(p,n);


    cout << "\n\n\nOutput for your memoization version of MCM algorithm for p being < 30,4,8,5,10,21,32,10,14,15,11,12,3,6,14,15,6,39,10,30>\n\n\n\n";

    memMCM(lab3p);













    cout << "------------------Part 5-6------------------------------------\n\n\n\n" << endl;
    cout << "Implement a breadth first search using an adjacency list.\n\n\n";
    cout << "Implement a breadth first search using an adjacency matrix.\n\n\n";





    cout << "------------------Part 7-8------------------------------------\n\n\n\n" << endl;
    cout << "Show the output for the bfs using the adjacency list on the graph below.\n\n\n";
    aList_BFS();


    cout << "\n\n Show the output for the bfs using the adjacency matrix on the graph below.\n\n\n";

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

    AdjacencyMatrix problem6_Matrix(14);

    for (int i=0;i<21;i++) {
        problem6_Matrix.add_edge(edgeList[i][0],edgeList[i][1]);
        problem6_Matrix.add_edge(edgeList[i][1],edgeList[i][0]);

    }
    problem6_Matrix.display();
    problem6_Matrix.AdjMatrixBFS('A');
    problem6_Matrix.AdjMatrixBFS('B');
    problem6_Matrix.AdjMatrixBFS('C');
    problem6_Matrix.AdjMatrixBFS('D');
    problem6_Matrix.AdjMatrixBFS('F');


	//code for creating adjacency matric for a user
    int nodes, max_edges;
    char origin, destin;
        cout<<"enter number of nodes: ";
        cin>>nodes;
        AdjacencyMatrix am(nodes);
        max_edges = nodes * (nodes - 1);
        for (int i = 0; i < max_edges; i++)
        {
            cout<<"enter edge (** to exit) must use capital letters: ";
            cin>>origin>>destin;
            if((origin == '*') && (destin == '*'))
                break;

			if (int(origin)-65 >= nodes || int(destin)-65 >= nodes) {
				cout << "\ninvalid edge\n";
			}
			else {

				am.add_edge(origin, destin);

			}
        }
        am.display();







}
