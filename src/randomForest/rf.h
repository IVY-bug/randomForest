//
//  rf.hpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#ifndef rf_h
#define rf_h
#include "tree.h"
#include "ioUtil.h"
#include "randint.h"
#include "message.h"
#include <mpi.h>
#define TREEN 50

using namespace std;

class rf {
public:
    classifier* trainOne();
    void train(int n);
    void train_mpi(int argc, char *argv[]);
    void predict(string fname, string outf);
    void predict_mpi(int argc, char *argv[], string fname, string outf);
    vector<int> predictOneLine(vector<int>& testVec);
    vector<int> bagData(double rate);
    static rf* getInstance(string fname);
    static int majority(vector<int> predicts, int& c0, int& c1);
    ~rf();
private:
    rf(string fname);
    static rf* instance;
    vector<classifier*> models;
    vector<vector<int> > dataSet;
    FileProcesser* fp;
};
#endif
