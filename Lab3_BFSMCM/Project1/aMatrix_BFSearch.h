#ifndef AMATRIX_BFSEARCH_H
#define AMATRIX_BFSEARCH_H
#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>

/*
 * Adjacency Matrix Class
 */
class AdjacencyMatrix
{
    private:
        int n;
       std::vector< std::vector<int> >adj;
        std::vector<bool> visited;
    public:
        AdjacencyMatrix(int n)
        {
            this->n = n;

            std::vector<bool> temp(n);
            visited = temp;


            std::vector< std::vector<int> > temp2(n, std::vector< int> (n));
            adj = temp2;

        }
        /*
         * Adding Edge to Graph
         */
        void add_edge(char o, char d)
        {
            int origin = int(o)-65;
            int destin = int(d)-65;



            if( origin > n || destin > n || origin < 0 || destin < 0 )
            {
                std::cout<<"Invalid edge!\n";
            }
            else
            {
                adj[origin][destin] = 1;
            }
        }
        /*
         * Print the graph
         */
        void display()
        {
            int i,j;

            for (int i=0;i<adj.size();i++) {
             std::cout << "  " << char(i+65);
            }

            std::cout << "\n";

            for(i = 0;i < n;i++)
            {
                std::cout << char(i+65) << " ";

                for(j = 0; j < n; j++)
                    std::cout<<adj[i][j]<<"  ";
                std::cout<<std::endl;
            }
        }





// Given an Adjacency Matrix, do a BFS on vertex "start"
void AdjMatrixBFS(char startNode)
    {

    int start = int(startNode)-65;
    std::cout << "\nDoing a BFS on an adjacency matrix.\n";


    // Create a "visited" array (true or false) to keep track of if we visited a vertex.
    std::vector <bool> visited (adj.size());

    // Create a queue for the nodes we visit.
    std::queue<int> q;

    // Add the starting vertex to the queue and mark it as visited.
    q.push(start);
    visited[start] = true;

    // While the queue is not empty..
    while(q.empty() == false)
        {
        int vertex = q.front();
        q.pop();

        // Doing +1 in the cout because our graph is 1-based indexing, but our code is 0-based.
        std::cout << char(vertex+65) << " ";

        // Loop through all of it's friends.
        for(int i = 0; i < adj[vertex].size(); i++)
            {

            // The neighbor is the column number, and the edge is the value in the matrix.
            int neighbor = i;
            int edge = adj[vertex][i];

            // If the edge is "0" it means this guy isn't a neighbor. Move on.
            if(edge == 0) { continue; }

            // If the friend hasn't been visited yet, add it to the queue and mark it as visited
            if(visited[neighbor] == false)
                {
                q.push(neighbor);
                visited[neighbor] = true;
                }
            }
        }
    std::cout << std::endl << std::endl;
    return;
}

};

#endif // AMATRIX_BFSEARCH_H
