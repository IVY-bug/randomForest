#ifndef run_h
#define run_h
#include "rf.h"
using namespace std;
void runOneThread(string trainData, string testData, string outputData);
void runMpi(string trainData, int argc, char * argv[]);
void runMpi2(string trainData, string testData, string outputData, int argc, char * argv[]);
#endif