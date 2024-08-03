/**
 * @file shortest.cpp
 * @author Griffin Nye
 * @brief CSC402 Dr. Spiegel Spring 2020- Constructs a graph using an adjacency list given 
 *        from an input file then computes the minimum cost path from the first vertex for each 
 *        vertex in the graph using Djikstra's algorithm, before outputting the graph, the minimum
 *        cost paths for each vertex along with their costs, and the number of microseconds it took
 *        to compute.
 * Due Date: 2/28/20
 * Assignment: #2
 */

#include <chrono>
#include <climits>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <queue>
#include <vector>

using namespace std;

struct Node {
public:

	/**getVal:
	 * @brief Retrieves the value of Node.val
	 * @return val the value stored in Node.val
	 */
	int getVal() const {
		return val;
	}//end getVal

	/**getCost:
	 * @brief Retrieves the value of Node.cost
	 * @return cost the value stored in Node.cost
	 */
	int getCost() const {
		return cost;
	}//end getCost

	/**setVal:
	 * @brief Sets the identifier for the recipient vertex of a directed edge
	 * @param vertex identifier for the recipient vertex of a directed edge
	 */
	void setVal(int vertex) {
		val = vertex;
	}//end setVal

	/**setCost:
	 * @brief Sets the cost to traverse edge directed to Node.val
	 * @param weight the cost/weight to traverse edge directed to Node.val
	 */
	void setCost(int weight) {
		cost = weight;
	}//end setCost

private:
	int val;
	int cost;
};//end Node

class nodeComparison {
public:
  /**operator()
	 * @param n1 the node to the left of the comparison operator
	 * @param n2 the node to the right of the comparison operator
	 */
  bool operator() (const Node& n1, const Node& n2) {
		return (n1.getCost() > n2.getCost());
	}//end operator()
};//end nodeComparison

//Function Prototypes
/**clearScreen:
 * @brief Clears the contents of the screen for improved readability.
 */
void clearScreen();

/**getInFile:
 * @brief Prompts the user for the input filename and returns the given string
 * @return filename the provided filename
 */
string getInFile();

/**getOutFile:
 * @brief Prompts the user for the output filename and returns the given string
 * @return filename the provided filename
 */
string getOutFile();

/**openFile:
 * @brief Attempts to open the file, returning error message upon failure
 * @param file filestream used to open the file- INPUT/OUTPUT
 * @param filename the name of the file to open- INPUT
 * @return fileOpen true = success, false = failure
 */
bool openFile(ifstream& file, const string filename);

/**getNumNodes:
 * @brief Retrieves the number of nodes from the file
 * @param file the input stream for the file- INPUT/OUTPUT
 * @return int(line) the number of nodes
 */
int getNumNodes(ifstream& file);

/**fillAdjList:
 * @brief Fills the adjacency list using the input text file
 * @param file the input stream for the file- INPUT/OUTPUT
 * @param adj the adjacency list- INPUT/OUTPUT
 * @return start the current time in microseconds at commencement of algorithm
 */
chrono::time_point< chrono::high_resolution_clock > 
	fillAdjList(ifstream& file, multimap<int, Node>& adj);

/**printAdjList:
 * @brief Prints the contents of the adjacency list
 * @param adj the adjacency list- INPUT
 */
void printAdjList(const multimap<int, Node>& adj);

/**findShortestPaths:
 * @brief Finds the shortest path from the source vertex to each vertex in the graph defaults and returns time of finish
 * @param adj the adjacency list- INPUT/OUTPUT
 * @param cost vector containing the cost of the shortest path for each vertex- INPUT/OUTPUT
 * @param num the number of vertices in the graph-INPUT
 * @param parent the array containing the parent of each vertex INPUT/OUTPUT
 * @return std::chrono::high_resolution_clock::now() current time in microseconds at completion of algorithm
 */
chrono::time_point< chrono::high_resolution_clock >
  findShortestPaths(multimap<int, Node>& adj, vector<int>& cost, int num, int parent[]);

/**findShortestPathsHelper:
 * @brief Helper function for findShortestPaths, Retrieves the current vertex's neighbors and updates costs
 * @param adj the adjacency list- INPUT/OUTPUT
 * @param cost vector containing the cost of the shortest path for each vertex- INPUT/OUTPUT
 * @param marked the marked array (0=unmarked, 1=marked)- INPUT/OUTPUT
 * @param pq the priority_queue- INPUT/OUTPUT
 * @param currVertex the current vertex- INPUT
 * @param currCost the current cost- INPUT
 * @param parent the array containing the parent of each vertex- INPUT/OUTPUT
 */
void findShortestPathsHelper(multimap<int, Node>& adj, vector<int>& cost, int* marked,
							 priority_queue<Node, vector<Node>, nodeComparison>& pq, int currVertex, 
							 int currCost, int parent[]);

/**initMarked
 * @brief Initalizes all vertices as unmarked(0) for Djikstra's
 * @param mark the marked array- INPUT/OUTPUT
 * @param num the number of vertices- INPUT
 */
void initMarked(int mark[], int num);

/**initCost:
 * @brief Initializes the cost of all vertices to INT_MAX for Djikstra's
 * @param cost vector containing the costs for all vertices- INPUT/OUTPUT
 */
void initCost(vector<int>& cost);

/**pathToString:
 * @brief Converts path into a string for printing of paths
 * @param parent the array containing the parent of each vertex- OUTPUT
 * @param elt the index of the element whose path is to be converted- INPUT
 * @return ss.str() string form of the path vector
 */
string pathToString(const int parent[], int elt);

/**printCostsAndPaths:
 * @brief Prints the minimum costs and minimum cost paths for all vertices
 * @param parent the array containing the parent of each vertex- OUTPUT
 * @param cost the cost list- INPUT/OUTPUT
 */
void printCostsAndPaths(vector<int>& cost, const int parent[]);

/**printTime:
 * @brief Prints the time in microseconds to perform the algorithm
 * @param ticks the number of microseconds
 */
void printTime(chrono::microseconds ticks);

/**recordTime:
 * @brief Records the time taken for the algorithm to be performed by appending to output file
 * @param file the output file
 * @param ticks the number of microseconds to perform the algorithm
 */
void recordTime(string file, chrono::microseconds ticks);

/**main
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 */
int main(int argc, char* argv[]) {
	ifstream input;
	string inFile;
	string outFile;
	multimap<int, Node> adjList;
	vector<int> costList;
	// map<int,vector<int> > pathMap;
	
	clearScreen(); //clear the screen

	if (argc == 1) {
		inFile = getInFile();
		outFile = getOutFile();
	} else if (argc == 2) {
		inFile = argv[1];
		outFile = getOutFile();
	} else if (argc == 3) {
		inFile = argv[1];
		outFile = argv[2];
	}//end if

	//check if opening file failed
	if (openFile(input, inFile) == false) {
		return -1; //EXIT_FAILURE
	}//end if
	
	//Get the number of nodes from the file
	int numNodes = getNumNodes(input);
	
	//Size Cost list and parent array
	costList.resize(numNodes);
	int parent[numNodes];
	
	//Initialize parent array
	for(int i = 0; i < numNodes; i++) {
		parent[i] = -2;
	}//end for
	
	//Populate the adjacency list
	auto start = fillAdjList(input, adjList);

	//Print the adjacency list
	//printAdjList(adjList);

	//Find all shortest paths from the source node
	auto end = findShortestPaths(adjList, costList, numNodes, parent);

	//Duration in microseconds of algorithm to be performed
	auto ticks = chrono::duration_cast<chrono::microseconds>(end - start);

	//Print min costs
	//printCostsAndPaths(costList, parent);

	//Print time
	//printTime(ticks);
	
	//Write time to file
	recordTime(outFile, ticks);
}//end main

//Clears the contents of the screen for improved readability.
void clearScreen() {
	system("clear");
}//end clearScreen

//Prompts the user for the input filename
string getInFile() {
	string filename;

	//Prompt for filename
	cout << "Please enter the name of the input file you wish to open: ";
	cin >> filename;

	return filename;
}//end getInFile

//Prompts the user for the output filename
string getOutFile() {
	string filename;
	
	//Prompt for filename
	cout << "Please enter the name of the output file you wish to populate: ";
	cin >> filename;
	
	return filename;
}//end getOutFile

//Attempts to open the file, returning an error message if needed.                                                                                                                                                                   
bool openFile(ifstream& file, const string filename) {
	bool fileOpen = true;

	//Open file
	file.open(filename.c_str());

	//If failure, display error message
	if (file.fail()) {
		cout << "There was an error when trying to open the file '" << filename << "'." << endl;
		cout << "Please make sure the file exists and is placed in the proper directory." << endl;
		fileOpen = false;
	}//end if

	return fileOpen;
}//end openFile

//Retrieves the number of nodes from the data file
int getNumNodes(ifstream& file) {
	string line;

	getline(file, line);

	return atoi(line.c_str());
}//end getNumNodes

//Fills the adjacency list using the input text file
chrono::time_point< chrono::high_resolution_clock >
  fillAdjList(ifstream& file, multimap<int, Node>& adj) {

	const string DELIMITER = " "; //Delimiter character
	string line;
	int vertex;

	auto start = chrono::high_resolution_clock::now();

	//Read file line by line
	while (getline(file, line)) {
		Node n;

		int pos = int(line.find(DELIMITER)); //Retrieve pos of first space
		vertex = int(line.substr(0, pos));  //Assign first field to vertex
		line.erase(0, pos + 1);		  //Delete first field from line
		pos = int(line.find(DELIMITER));	    //Retrieve pos of second space
		n.setVal(stoi(line.substr(0, pos))); //Set Node.val to second field
		line.erase(0, pos + 1);						   //Delete second field from line
		n.setCost(stoi(line));						   //Set Node.cost to third field

		adj.insert(make_pair(vertex, n));    //Map vertex to node and insert into adj
	}//end while

	return start;
}//end fillAdjList    

//Prints the contents of the adjacency list
void printAdjList(const multimap<int, Node>& adj) {
	multimap<int, Node>::const_iterator itr;

	cout << "Graph:" << endl;
	cout << setw(13) << "Start_Vertex" << right << setw(12) << "Dest_Vertex"
		<< setw(5) << "Cost" << endl;

	//Traverse through vertices
	for (itr = adj.begin(); itr != adj.end(); itr++) {
		cout << right << setw(7) << itr->first << setw(12) << itr->second.getVal() << setw(11)
			<< itr->second.getCost() << endl;
	}//end for

	cout << endl;
}//end printAdjList

//Finds the min cost path from the source vertex for each vertex
chrono::time_point< chrono::high_resolution_clock > 
  findShortestPaths(multimap<int, Node>& adj, vector<int>& cost, int num,
										int parent[]) {
		
	priority_queue<Node,vector<Node>,nodeComparison> pq; 
		 //Priority Queue for storing adjacent vertices
	int* marked = new int[num];      	 //Array for marking vertices
	int currCost;                      //Stores the current cost
	int currVertex;                    //Stores the current vertex
	Node n;

	initMarked(marked, num); 			 	//Initialize all vertices as unmarked(0)
	initCost(cost); 		 	    	//Initalize all costs to INT_MAX

	cost[0] = 0;    										//Set the cost of the first vertex to 0

	//Create Node for first vertex
	n.setCost(0);
	n.setVal(1);
	pq.push(n); 												//Insert node into Priority Queue
	parent[0] = -1;                    //Mark as the source vertex
	

	//Loop while priority_queue contains elements
	while (!pq.empty()) {
		currVertex = pq.top().getVal();  //Retrieve the Min Cost Vertex
		currCost = cost[currVertex - 1]; //Retrieve the cost to reach the Min Cost Vertex
	
		findShortestPathsHelper(adj, cost, marked, pq, currVertex, currCost, parent); //Call helper
	}//end while

	return (chrono::high_resolution_clock::now() );
}//end findShortestPath

//Retrieves the current vertex's neighbors and updates costs
void findShortestPathsHelper(multimap<int, Node>& adj, vector<int>& cost, int *marked, 
							 priority_queue<Node, vector<Node>, nodeComparison> & pq, int currVertex, 
							 int currCost, int parent[]) {

	//If vertex is unmarked, retrieve its cost, mark it, and remove from pq
	if (marked[currVertex - 1] == 0) {
		marked[currVertex - 1] = 1;      //Mark the vertex as visited
		pq.pop();                        //Remove the Min Cost Vertex from pq  

		//First adjacent vertex to Min Cost Vertex
		multimap<int, Node>::iterator itr1 = adj.lower_bound(currVertex);
		//Last adjacent vertex to Min Cost Vertex
		multimap<int, Node>::iterator itr2 = adj.upper_bound(currVertex);

		//Insert all of the Min Cost Vertex's neighbors into pq
		while (itr1 != itr2) {

					//Only insert into pq if vertex is unmarked
					if (marked[itr1->second.getVal() - 1] == 0) {

						//Update Costs
						if (currCost + itr1->second.getCost() < cost[(itr1->second.getVal() )- 1]) {
							cost[itr1->second.getVal() - 1] = currCost + (itr1->second.getCost() );
							itr1->second.setCost(cost[ (itr1->second.getVal() ) - 1]);
							//Set currentVertex as the parent
							parent[itr1->second.getVal() - 1] = currVertex;
						}//end if

						pq.push(itr1->second);
					}//end if

					itr1++; //Move to the next element in multimap
			}//end while
	}
	else {
		pq.pop(); //Minimum cost path already found for vertex, remove from pq
	}//end if
}//end findShortestPathsHelper

//Initializes all vertices as unmarked
void initMarked(int mark[], int num) {
	for (int i = 0; i < num; i++) {
		mark[i] = 0;
	}//end for
}//end initMarked

//Initializes all costs as INT_MAX
void initCost(vector<int>& cost) {
	for (int i = 0; i < cost.size(); i++) {
		cost[i] = INT_MAX;
	}//end for
}//end initCost

//Converts path into a string for insertion into pathMap
string pathToString(const int parent[], int elt) {
	stringstream ss;
	vector<int> path;
	vector<int>::iterator itr;
	int prev;
	
	path.push_back(elt+1); //Add dest vertex to path
	prev = parent[elt];    //Determine parent
	
	//Loop until starting vertex found
	while(prev!=-1) {
		
		if(prev != -2) {
		path.push_back(prev); 	//Add parent vertex to path 
		} else {
			return "n/a";        //Return n/a if vertex is disconnected from graph
		}//end if-else
			
		prev = parent[prev-1]; //Find next parent
	}//end while
	
	//Reverse order of stored path
	for(itr = path.end()- 1; itr >= path.begin(); itr--) {
		ss << *itr << " ";
	}//end for
	
	return ss.str();
}//end pathToString

//Prints the cost list
void printCostsAndPaths(vector<int>& cost, const int parent[]) {
	cout << "Min Costs:" << endl;

	for (int i = 0; i < cost.size(); i++) {
		cout << right << setw(11) << "Vertex " << i+1 << ":" << setw(11);
		
		//Print n/a for unconnected vertices
		if (cost[i] != INT_MAX) {
			cout << cost[i];
		}
		else {
			cout << "n/a";
		}//end if
		
		//Print the path
		cout << left << " Path: ";
		cout << pathToString(parent, i) << endl;
	}//end for

	cout << endl;
}//end printCosts

//Prints the time taken for the algorithm to be performed
void printTime(chrono::microseconds ticks) {
	cout << "The algorithm took " << ticks.count() << " microseconds to perform." << endl;
}//end printTime

//Records the time taken for the algorithm to be performed by appending to file
void recordTime(string file, chrono::microseconds ticks) {
 ofstream out;

 out.open(file.c_str(), ofstream:: app);
 
 if (out.is_open() ) {
	 out << ticks.count() << " ";
 } else {
	 cerr << "Error writing to file";
 }//end if-else
	 
 out.close();
}//end recordTime
