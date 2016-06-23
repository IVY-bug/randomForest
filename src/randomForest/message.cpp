//
//  message.cpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#include "message.h"
#include <iostream>

void ErrorMesg(string errorMsg, bool exit_) {
    cout << errorMsg << endl;
    if (exit_)
        exit(0);
}


void Info(string info) {
    cout << info << endl;
}

void Info(double n) {
    cout << n << endl;
}

void Info(int n) {
	cout << n << endl;
}