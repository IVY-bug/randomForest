//
//  rf.cpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#include "rf.h"
#include <iostream>

rf* rf:: instance = nullptr;

classifier* rf::trainOne() {
    tree::bagData = bagData(1);
    vector<int> features;
    for (int i = 0; i < dataSet[0].size()-1; i++)
        features.push_back(i);

    vector<int> theindex;
    for (int i = 0; i < tree::bagData.size(); i++) {
        theindex.push_back(i);
    }
    tree* t = (new tree())->buildTree(theindex, features, 0);
    classifier* ret = new classifier(t);
    delete t;
    return ret;
}
/*data bag*/
vector<int> rf::bagData(double r) {
    vector<int> ret;
    int tmp;
    int n = r*dataSet.size();
    for (int i = 0; i < n; i++) {
        tmp = randint(n);
        ret.push_back(tmp);
    }
    return ret;
}


rf::rf(string fname) {
    this->fp = FileProcesser::getInstance();
    this->dataSet = fp->readFile(fname);
}

rf* rf::getInstance(string fname) {
    if(instance == nullptr) {
        instance = new rf(fname);
    }
    return instance;
}

void rf::train(int n) {
    tree::data = &dataSet;
    for (int i = 0; i < n; i++) {
        Info("Training tree");
        models.push_back(trainOne());
    }
}

void rf::train_mpi(int argc, char *argv[]) {
    int isInitialized = 0;
    MPI_Initialized(&isInitialized);
    if (!isInitialized) {
        MPI_Init(&argc, &argv);
    }
    
    int size, rank;
    MPI_Status  status;
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int treeTrainNum = 0;

    while (true) {
        if (rank == 0) {
            if (treeTrainNum >= TREEN) {
                treeTrainNum = -1;
            }
            if (treeTrainNum == -1) {
                for (int i = 1; i < size; i++) {
                    MPI_Send(&treeTrainNum, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
                break;
            }
            for (int i = 1; i < size; i++) {
                treeTrainNum++;
                MPI_Send(&treeTrainNum, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            int finishTrainTreeNum = 0;
            for(int i = 1; i < size; i++) {
                MPI_Recv(&finishTrainTreeNum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            }
            Info("Finish training tree:");
            Info(treeTrainNum-size+2);
            Info(treeTrainNum);
        }
        else {
            MPI_Recv(&treeTrainNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if (treeTrainNum == -1)
                break;
            this->train(1);
            int size = models.size();
            MPI_Send(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }
}

int rf::majority(vector<int> predicts, int& cnt0, int& cnt1) {
    cnt0 = 0;
    cnt1 = 0;
    for (int i = 0; i < predicts.size(); i++) {
        if (predicts[i] == 0)
            cnt0++;
        else
            cnt1++;
    }
    return cnt1 > cnt0;
}

vector<int> rf::predictOneLine(vector<int>& testVec) {
    vector<int> aresult;
    for (int j = 0; j < models.size(); j++) {
        aresult.push_back(models[j]->classify(testVec));
    }
    return aresult;
}

void rf::predict(string fname, string outf) {
    if (models.size() == 0) {
        Info("No model! please train first!");
        return;
    }
    Info("Predicting...");
    int tmp, suc = 0, cnt1, cnt0;
    vector<int> towrite;
    vector<vector<int> > toPredict = fp->readFile(fname);
    for (int i = 0; i < toPredict.size(); i++) {
        tmp = majority(predictOneLine(toPredict[i]), cnt0, cnt1);
        if (tmp == toPredict[i][toPredict[i].size()-1])
            suc++;
        towrite.push_back(tmp);
    }
    Info("suc rate:");
    Info((double)suc / (double)toPredict.size());
    fp->writeToFile(towrite, outf);
}

void rf::predict_mpi(int argc, char *argv[], string fname, string outf) {
    int isInitialized = 0;
    MPI_Initialized(&isInitialized);
    if (!isInitialized) {
        MPI_Init(&argc, &argv);
    }
    
    int size, rank;
    MPI_Status  status;
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<vector<int> > toPredict = fp->readFile(fname);
    int dataSize = toPredict.size();
    if(rank == 0) {
        
        int beginPredict = 1;
        Info(size);
        for (int i = 1; i < size; i++) {
            MPI_Send(&beginPredict, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int* result = new int[dataSize*2];
        int* cnt0 = new int[dataSize];
        int* cnt1 = new int[dataSize];
        for (int i = 0; i < dataSize; i++) {
            cnt0[i] = cnt1[i] = 0;
        }
        for (int i = 1; i < size; i++) {
            MPI_Recv(result, dataSize*2, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            for (int j = 0; j < dataSize; j++) {
                cnt0[j] += result[j*2];
                cnt1[j] += result[j*2+1];
            }
        }
        vector<int> ans;
        int suc = 0;
        for (int j = 0; j < dataSize; j++) {
            ans.push_back(cnt1[j]>cnt0[j]);
            if (ans[j] == toPredict[j][toPredict[j].size()-1])
                suc++;
        }
        delete []result;
        delete []cnt0;
        delete []cnt1;
        Info("suc rate:");
        Info((double)suc / (double)toPredict.size());
        fp->writeToFile(ans, outf);

    } else {
        int beginTrain;
        MPI_Recv(&beginTrain, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        
        int* result = new int[dataSize*2];  
        int cnt0, cnt1;
        for (int i = 0; i < dataSize; i++) {
            majority(predictOneLine(toPredict[i]), cnt0, cnt1);
            result[i*2] = cnt0;
            result[i*2+1] = cnt1;
        }
        MPI_Send(result, dataSize*2, MPI_INT, 0, 1, MPI_COMM_WORLD);
        delete []result;
    }
}

rf::~rf() {
    delete fp;
    for (int i = 0; i < models.size(); i++) {
        delete models[i];
    }
}

