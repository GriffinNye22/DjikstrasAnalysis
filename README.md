# Djikstra's Analysis
An implementation of Dijkstra's Shortest Path Algorithm, designed to compute the shortest path from a start vertex to all other nodes in a graph. This implementation serves as a basis for performance analysis, documenting how variations in the number of vertices and in-degrees affect execution time.

|                       |                                                                                                                               |
|----------------------:|-------------------------------------------------------------------------------------------------------------------------------|
|**Last Modified Date:**| 4/18/2020                                                                                                                     |
|**Last Modifications:**| Fine-tuned input file generation to prepare for Performance Analysis                                                          |
|     **Documentation:**| [Doc Site](https://codedocs.xyz/GriffinNye22/DjikstrasAnalysis/)                                                              |  
|          **Platform:**| UNIX                                                                                                                          |
|          **Language:**| C++                                                                                                                           |

## Design Details

**GenGraph.cpp:**  
GenGraph.cpp generates a single input file for the shortest.cpp program given the number of vertices in the graph (|V|), the degree of each vertex, the maximum cost for traveling between any two vertices, and a seed for the random number generator, in that order. If degree is one less than the number of vertices, then a fully connected graph will be generated, where the only randomness that occurs is the cost for traveling between vertices. Otherwise, neighbor vertices will be randomly generated for each vertex in the graph in conjunction with a randomly generated cost.
 
**cExec.cpp:**  
cExec.cpp utilizes GenGraph.cpp to generate multiple input files, run them through shortest.cpp, and record their execution times, given the executable name (gen), the number of graphs to generate, the number of vertices in each graph (|V|), the degree of vertices in each generated graph, the maximum cost for traveling between any two vertices in each graph, and a seed for the random number generator, in that order. cExec takes these arguments and generates an output file named x.cpp, where x is the number of vertices in the generated graphs, and appends a row header to the output file in the format \<# of vertices\>.\<degree\>.\<maxCost\>. 

cExec then forks child processes to run the provided arguments through genGraph.cpp to generate an input file. The parent process waits for its child process to finish, then passes the input file generated by the child process and the name of the output file it generated through shortest.cpp, which executes the algorithm and records the execution time in the output file. This process is repeated based on the specified number of graphs to generate.

**shortest.cpp:**  
For my adjacency list, I utilized a multimap consisting of pairs of integers and a custom-defined struct named Node which contains data members val (the destination vertex) and cost. 
	 
In order to maintain encapsulation, I set both of Node's data members to private and implemented member functions to set and retrieve these data members
		
For the design of the function running Djikstra's algorithm (findShortestPaths() ) I utilized a few data structures: the adjacency list is represented as a multimap of nodes mapped to integers; the minimum cost for each vertex is stored in a vector of integers, the neighbors of traversed vertices are stored in a priority_queue of Nodes, and marked vertices are stored in an integer array pointed to by the marked pointer.

I later decided to split the FindShortestPaths() function into two functions: itself and a helper function; for both efficiency and organizational purposes. The call to the helper function adds two more data elements: currVertex, the current min cost vertex, and currCost, the cost of traversing from the source node to the current min cost vertex.
	 
FindShortestPaths() declares and calls functions to initialize the data structures needed to perform Djikstra's Algorithm. It then creates a node for the root vertex and inserts it into the priority queue before passing the current vertex and current cost along to the FindShortestPathsHelper() function to begin graph traversal.
	 
FindShortestPathsHelper() traverses the neighbors of the graph and updates costs where needed. First, it marks the current vertex, if it hasn't done so already, before popping the top element off of the priority_queue. It then traverses all neighbors of the current vertex using iterators, checking for marked vertices and lesser costs along the way, before pushing the respective nodes into the priority_queue.

## Performance Analysis
For performance analysis, I utilized cExec.cpp to generate 11 graph input files for each combination of in-degree and number of vertices, ranging from 2 to 10. This analysis aimed to examine how the number of vertices and in-degree affect the execution time of Dijkstra's algorithm. Additionally, I generated 11 graph input files per combination to investigate if cache impacts the algorithm’s performance as a secondary hypothesis. More details on this process and the findings are available in "Research Paper/ExploringDijkstrasAlgorithm.pdf".

## Info to run program:
**CEXEC**  
```cpp
g++ -o cexec cExec.cpp
cexec <executable> |V| <in-degree> <max cost> <seed>
```
**GEN GRAPH**  
```cpp
g++ -o gen GenGraph.cpp
```
