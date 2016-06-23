#!/bin/bash
for((j=0;j < 21; j++));
do
rm data/part$j
done

rm data/trainData
rm data/testData
rm data/outputData
rm data/result.txt


touch data/trainData
touch data/testData
touch data/result.txt
touch data/outputData