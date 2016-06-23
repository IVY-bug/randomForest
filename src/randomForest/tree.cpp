//
//  tree.cpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#include "tree.h"
#include <iostream>
int tree::max_depth = 10;
int tree::bag_n = 40;
vector<vector<int>>* tree::data = nullptr;
vector<int> tree::bagData = vector<int>();
tree::tree() {
    l = r = nullptr;
    depth = 0;
    label = -1;
    splitf = -1;
}

tree::~tree() {
    clearTree();
}

tree* tree::buildTree(vector<int> dataSet, vector<int> features, int depth) {
    this->myindex = dataSet;
    this->fs = features;
    this->depth = depth;
    
    //reach the max_depth specified
    if (//(this->depth == max_depth) ||
        //no more feature to split
        ((this->fs).size()) == 0 ||
        //labels in dataSet are identical
        (labelTheSame(dataSet)) ||
        (dataSet.size() == 1)
        ) {
        this->splitf = -1;
        this->label = majorityCnt(getLabels(dataSet));
        this->l = this->r = nullptr;
        return this;
    }
    
    vector<int> myfeatures = bagf(features);
    vector<int> l_data, r_data;
    int bestf_axis = bestToSplit(myfeatures, l_data, r_data);
    if (l_data.size() && r_data.size()) {
        this->splitf = fs[bestf_axis];
        vector<int> sub_f(fs);
        deleteF(sub_f, bestf_axis);
        this->l = (new tree())->buildTree(l_data, sub_f, depth+1);
        this->r = (new tree())->buildTree(r_data, sub_f, depth+1);
        return this;
    } else {
        this->splitf = -1;
        this->label = majorityCnt(getLabels(dataSet));
        this->l = this->r = nullptr;
        return this;
    }
}

void tree::clearTree() {
    if (this->l) {
        delete this->l;
        this->l = nullptr;
    }
    if (this->r) {
        delete this->r;
        this->r = nullptr;
    }
    vector<int> a;
    this->fs.swap(a);
    vector<int> b;
    this->myindex.swap(b);
}

int tree::bestToSplit(vector<int> myfeatures, vector<int>& l_data, vector<int>& r_data) {
    double baseGain = calcShannonEnt(myindex);
    double bestGain = 0.0;
    double infoGain;
    vector<int> tmp_l, tmp_r;
    int best_f = 0;
    for (int i = 0; i < myfeatures.size(); i++) {
        splitData(fs[myfeatures[i]], tmp_l, tmp_r);
        infoGain = baseGain
                   - double(tmp_l.size())/myindex.size()*calcShannonEnt(tmp_l)
                   - double(tmp_r.size())/myindex.size()*calcShannonEnt(tmp_r);
        if (infoGain > bestGain) {
            best_f = myfeatures[i];
            bestGain = infoGain;
            l_data = tmp_l;
            r_data = tmp_r;
        }
    }
    return best_f;
}


void tree::splitData(int axis, vector<int>& l_data, vector<int>& r_data) {
    l_data.clear();
    r_data.clear();
    for (int i = 0; i < myindex.size(); i++) {
        if ((*data)[bagData[myindex[i]]][axis] == 0) {
            l_data.push_back(myindex[i]);
        } else {
            r_data.push_back(myindex[i]);
        }
    }
}


vector<int> tree::getLabels(vector<int>& dataSet) {
    vector<int> labels;
    if (dataSet.size() == 0)
        return labels;
    int label_axis = (*data)[bagData[dataSet[0]]].size()-1;
    for (int i = 0; i < dataSet.size(); i++) {
        labels.push_back((*data)[bagData[dataSet[i]]][label_axis]);
    }
    return labels;
}



void tree::deleteF(vector<int>& features, int axis) {
    vector<int>::iterator itr;
    int i;
    for (i = 0, itr = features.begin();itr!= features.end();i++) {
        if (i == axis) {
            features.erase(itr);
        } else {
            itr++;
        }
    }
}

double tree::calcShannonEnt(vector<int>& dataSet) {
    vector<int> labels = getLabels(dataSet);
    int cnt0 = 0, cnt1, tot = dataSet.size();
    double ret = 0.0;
    for (int i = 0; i < labels.size(); i++) {
        if (labels[i] == 0)
            cnt0++;
    }
    cnt1 = tot - cnt0;
    double p0 = double(cnt0)/tot;
    double p1 = double(cnt1)/tot;
    ret = -(p0*log2(p0)+p1*log2(p1));
    return ret;
}


bool tree::labelTheSame(vector<int>& dataSet) {
    vector<int> labels = getLabels(dataSet);
    for (int i = 0; i < labels.size()-1; i++) {
        if (labels[i]+labels[i+1] == 1)
            return false;
    }
    return true;
}

int tree::majorityCnt(vector<int> votes) {
    int v0 = 0, v1 = 0;
    for (int i = 0; i < votes.size(); i++) {
        if (votes[i] == 0)
            v0++;
        else
            v1++;
    }
    return v1 > v0;
}

vector<int> tree::bagf(vector<int>& features) {
    int l = features.size();
    int i, tmp;
    vector<int> ret;
    vector<int> index;
    vector<int>::iterator it = index.begin();
    for (i = 0; i < l; i++) {
        index.push_back(i);
    }
    int n = bag_n;
    if (n > l)
        n = l;
    for (i = 0; i < bag_n; i++) {
        it = index.begin();
        tmp = randint(l-i);
        ret.push_back(index[tmp]);
        index.erase(it+tmp);
    }
    return ret;
}

tree* tree::getLsub() {
    return this->l;
}

tree* tree::getRsub() {
    return this->r;
}

int tree::getSplitf() {
    int tmp = this->splitf;
    if (tmp != -1)
        return tmp;
    return this->label;
}

classifier::classifier(tree* t) {
    this->splitf = t->getSplitf();
    if (t->getLsub() != nullptr) {
        this->l = new classifier(t->getLsub());
    } else {
        this->l = nullptr;
    }
    if (t->getRsub() != nullptr) {
        this->r = new classifier(t->getRsub());
    } else {
        this->r = nullptr;
    }
}

int classifier::classify(vector<int> testVec) {
    classifier* cur = this;
    int splitf;
    while (cur->l != nullptr || cur->r != nullptr) {
        splitf = cur->splitf;
        if (testVec[splitf] == 0) {
            cur = cur->l;
        } else {
            cur = cur->r;
        }
    }
    return cur->splitf;
}

classifier::~classifier() {
    if (this->l != nullptr) {
        delete this->l;
        this->l = nullptr;
    }
    if (this->r != nullptr) {
        delete this->r;
        this->r = nullptr;
    }
}




