//
//  message.hpp
//  randomforest
//
//  Created by apple on 16/6/16.
//  Copyright © 2016年 apple. All rights reserved.
//

#ifndef message_h
#define message_h

#include <string>
using namespace std;

void ErrorMesg(string errorMsg, bool exit_);

void Info(string info);

void Info(double n);

void Info(int n);


#endif /* message_hpp */
