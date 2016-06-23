//
//  main.cpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#include <iostream>
#include "run.h"
using namespace std;
int main(int argc, char * argv[]) {
    string traindata = "/Users/apple/Desktop/RF/data/trainData";
    string testdata = "/Users/apple/Desktop/RF/data/testData";
    string outputdata = "/Users/apple/Desktop/RF/data/outputData";
    srand((int)time(0));
    //runOneThread(traindata, testdata, outputdata);
    runMpi(traindata, argc, argv);
    runMpi2(traindata, testdata, outputdata, argc, argv);
    MPI_Finalize();
    return 0;
}
