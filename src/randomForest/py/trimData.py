import readfile
from readfile import *
import featureAnalize
from featureAnalize import *

if __name__ == '__main__':
	data, features = readData2('data/part1')
	outf = 'data/trainData'
	output(outf, data)

	fname = 'data/part2'
	outf = 'data/testData'
	toPredict = open(fname).readlines()
	testData = []
	for line in toPredict:
		labels = []
		vector = rearrange(line, features)
		testData.append(vector)
	output(outf, testData)