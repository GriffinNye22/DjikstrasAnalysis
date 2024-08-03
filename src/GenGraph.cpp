/// File: GraphGen.cpp
/// Quick and Dirty Generator of Graphs Represented by Adjacency Lists
/// Written by Dr. Spiegel
///
/// This program will generate a graph with the number of vertices, degree 
///  (in and out will be the same for all vertices), and maximum cost (cost
///  is positive and of type int) provided by the user.
///
/// The user will also provide a seed for the random number generator.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

// genEdges creates the edges for each vertex in the data file. 
//	It generates d edges for each vertex
//	The data file will have |V| * d total edges
//	outFile - Import/Export (by rule)
//	All other parameters are import only
void genEdges(ofstream &outFile,int vertex,int vertices,int degree,int maxCost);

// If fully connected, only thing random is cost of edges
void fullyConnected(ofstream &outFile,int vertices,int maxCost);

int main(int argc,char **argv)
{ if (argc != 6) {
    cerr << "Form: gen <data file>  |V| <degree> <max cost> <seed>\n";
    return(-1);
  }
  // Set up data file
  ofstream outFile;
  outFile.open(argv[1]);	// We assume success (recall: quick and dirty)
  int vertices=atoi(argv[2]),
      degree=atoi(argv[3]),
      maxCost=atoi(argv[4]),
      seed=atoi(argv[5]);
  if (degree>=vertices) {
    cerr << "\nDegree of each vertex must be less than |V|. Bye.\n";
    return(-2);
  }  

  srand(seed);			// Seed random number generator
  //   Let's write the file
  outFile << vertices << endl;
  // Check for fully connected
  if (degree==vertices-1) 
    fullyConnected(outFile,vertices,maxCost);
  else for (int vertexNum=1;vertexNum<=vertices;vertexNum++)
    genEdges(outFile,vertexNum,vertices,degree,maxCost);
  outFile.close();		// Done  
  return(0);  
}    
    
// genEdges creates the edges for each vertex in the data file. 
//	Generate edges. None can end at value edge, since that's the source
//	So, there are vertex-1 possibilities.
//	 Find a random number between 1 and vertices-1. 
//	 If it is greater than or equal to edge, add one to it.
void genEdges(ofstream &outFile,int vertex,int vertices,int degree,int maxCost)
{  vector<bool> taken(degree);
   for (int idx=0;idx<degree;idx++)
     taken[idx]=false;	
   //cout << "X" << taken;
   // Need to be sure a destination doesn't repeat
   for (int edgeNum=0;edgeNum<degree;edgeNum++) {
     int dest;
     do {
       dest=rand() % (vertices-1) + 1;
     } while (taken[dest-1]);  
     taken[dest-1]=true;
     // We don't want an edge to loop (that's why (vertices-1) above)
     if (dest >= vertex) 	// Move up one to account for no loop
       dest++;
     // OK. We have a terminus; Write it.
     outFile <<  vertex << " " << dest << " ";
     // Need a cost. get one between 1 and Max.
     outFile << rand() % maxCost + 1 << endl;
   }
}    

// If fully connected, only thing random is cost of edges
void fullyConnected(ofstream &outFile,int vertices,int maxCost)
{ for (int vertex=1;vertex<=vertices;vertex++) 
    for (int edge=1;edge<=vertices;edge++)
      if (edge != vertex)
        outFile << vertex << " " << edge << " " << rand() % maxCost + 1 << endl;
}	

