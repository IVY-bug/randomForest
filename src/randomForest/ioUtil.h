//
//  ioUtil.hpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#ifndef ioUtil_h
#define ioUtil_h

#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "message.h"

using namespace std;

class FileProcesser {
public:
    ~FileProcesser();
    
    vector<vector<int> > readFile(const string &filepath);
    void writeToFile(const vector<int> &content, const string &filepath);
    
    static FileProcesser* getInstance();
    
private:
    FileProcesser();
    vector<int> split(const string &s, char delim);
    
private:
    static FileProcesser* instance;
};


#endif /* ioUtil_hpp */
