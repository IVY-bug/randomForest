#!/bin/bash
#清理残留文件
sh clean.sh
#建立新文件
sh job.sh
#对数据分区
python src/randomForest/py/createData.py
#抽取特征
python src/randomForest/py/trimData.py

#生成makefile并编译
cd src
cmake .
make clean
make
cd ..

#用mpich执行5个进程
time mpirun -np 5 bin/main

#整理结果文件的格式并写入result.txt
python src/randomForest/py/reformData.py

