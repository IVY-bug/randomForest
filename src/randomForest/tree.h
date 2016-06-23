//
//  tree.hpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#ifndef tree_h
#define tree_h
#include <vector>
#include <stack>
#include <math.h>
#include "message.h"
#include "randint.h"

class tree {
public:
    static int max_depth, bag_n;
    static vector<vector<int> >* data;
    static vector<int> bagData;
    tree();

    ~tree();

    tree* buildTree(vector<int> dataSet, vector<int> features, int depth);
    
    void splitData(int axis, vector<int>& l_data, vector<int>& r_data);
    
    int bestToSplit(vector<int> myfeatures, vector<int>& l_data, vector<int>& r_data);
    
    static vector<int> bagf(vector<int>& features);
    
    static double calcShannonEnt(vector<int>& dataSet);

    static int majorityCnt(vector<int> votes);
    
    static bool labelTheSame(vector<int>& dataSet);
    
    static vector<int> getLabels(vector<int>& dataSet);

    static void deleteF(vector<int>& features, int axis);

    tree* getLsub();

    tree* getRsub();

    int getSplitf();

private:
    void clearTree();
    tree* l;
    tree* r;
    vector<int> myindex;
    vector<int> fs;
    int label;
    int depth;
    int splitf;
};

class classifier {
public:
    classifier(tree* t);
    ~classifier();
    int classify(vector<int> testVec);
private:
    int splitf;
    classifier* l;
    classifier* r;
};


#endif
