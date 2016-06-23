//
//  ioUtil.cpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#include "ioUtil.h"
#include <iostream>
FileProcesser* FileProcesser::instance = nullptr;


FileProcesser::~FileProcesser() {
    if (instance != nullptr)
        delete instance;
}

FileProcesser::FileProcesser() { }

FileProcesser* FileProcesser::getInstance() {
    if (instance == nullptr)
        instance = new FileProcesser();
    return instance;
}


vector< vector<int> > FileProcesser::readFile(const string &filePath) {
    vector<vector<int>> result;
    ifstream in(filePath);
    char buff[5000];
    if (in.is_open()) {
        while (in.getline(buff, 4999)) {
            result.push_back(split(buff, ' '));
        }
    }
    return result;
}

vector<int> FileProcesser::split(const string &s, char delim) {
    stringstream ss(s);
    vector<int> elems;
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(int(item[0]-'0'));
    }
    return elems;
}

void FileProcesser::writeToFile(const vector<int> &content, const string &filepath) {
    ofstream out(filepath,std::ofstream::out);
    for (int i = 0; i < content.size(); i++) {
        if (i == 0) {
            out << content[i];
        } else {
            out << '\n' << content[i];
        }
    }
    
}