// ExecLocal.cpp
// Batch Processor for Generating and Testing Graphs
// No file name is passed in. 
// The file name will be created according to the last 4 command line arguments
// Modified by: Griffin Nye


//#include <process.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <wait.h>
#include <cstring>
#include <string.h>
#include <fstream>
#include <iostream>

#define SEED_ARG 6     // argument number of seed for GenGraph (starting from 0)

using namespace std;

int main(int argc,char **argv)
{int termstat,pid,count;
 string outFile = string(argv[3]) + ".txt"; 
 ofstream out;
 
 out.open(outFile.c_str(), ofstream::app);
 
 //Write Line Label to file
 if (out.is_open() ) {
    out << argv[3] << "." << argv[4] << "." << argv[5] << " ";
    out.close();
 } else {
   cerr << "Error writing to file";
 }//end if

// Command line arguments are: 
//		<executable name> <#graphs> |V| <degree> <max cost> <seed>

 char **CommandArgs,thisSeed[5];
 
 int RandSeed=atoi(argv[SEED_ARG]); // Initial Seed from user
 cout << "Rand Seed (argv[SEED_ARG]) is " << RandSeed << endl;
 // argv[1] is # executions; store in local variable
 int numTimes=atoi(argv[2]);
 


 for (count=0;count<numTimes;count++) {
  sprintf(thisSeed,"%2d",(RandSeed+count*7)%100);
  // New seed for creating the graph
  strcpy(argv[SEED_ARG],thisSeed);
  // Create the file name; it will go in argv[2], where the #graphs was 
  // Build name of data file based on command line arguments
  // Must do before the fork, so the parent has the data file's name
  //	Parent will use data file after wait() returns.
  argv[2]=new char[40];  // must reinit to avoid contaminating neighbors
  sprintf(argv[2],"%s.%s.%s.%s",argv[3],argv[4],argv[5],thisSeed);
  if ((pid=fork())<0) {
	perror("fork");
	exit(-1);
  }
  else if (!pid) { // child process
   //	(we stored that elsewhere)
   if (argc>2) {  // print the command string upcoming
    for (int j=0;j<argc;j++)
   	cout << argv[j] << " ";
    cout << endl;
    pid=execv(argv[1],argv+1);
    cerr << "execv Failed";    
   }
  }
  else { // parent just waits
    wait(&termstat);
    // OK, child process has created graph. We can now process it.
    cerr << "Opening " << argv[2] << endl;
    // execl("vi","vi",argv[2],0);
    char viStr[40]; 
    sprintf(viStr,"shortest %s %s",argv[2], outFile.c_str());
    system(viStr);
  }
 } // for loop
 out.open(outFile.c_str(), ofstream::app);
 
 if(out.is_open() ) {
  out << endl;
 }//end if
 
 out.close();
 return(0);
} // main

