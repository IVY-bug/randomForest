import readfile
import tree
import featureAnalize
import random
import time
from random import *
from featureAnalize import *
from tree import *
from readfile import *
import datetime
import pp

def bag(dataSet):
	ret = []
	for i in range(len(dataSet)):
		pos = random.randint(0,len(dataSet)-1)
		ret.append(dataSet[pos])
	return ret

def train(treen, dataSet, features):
	models = []
	for i in range(treen):
		print "training tree " + str(i)
		subData = dataSet
		models.append(createTree(subData, range(len(subData[0])-1), 0))
	return models

def predict(fname, modelsandfeatures, outf):
	models, features = modelsandfeatures
	result = []
	suc = 0
	toPredict = open(fname).readlines()
	for line in toPredict:
		labels = []
		vector = rearrange(line, features)
		for model in models:
			label = classify(model, vector)
			labels.append(label)
		l = majorLabel(labels)
		if l == vector[-1]:
			suc += 1
		result.append(l)
	outf = open(outf, 'w')
	for label in result:
		outf.write(str(label)+'\n')
	outf.close()
	return float(suc)/float(len(toPredict))

def majorLabel(labels):
	if labels.count('1') > len(labels)/2:
		return '1'
	return '0'

if __name__ == '__main__':
	dataSet, features = readData2('part1')
	models = train(10, dataSet, features)
	rate = predict('part1', (models, features), 'part1_out')
	print rate
#It took me a-lot-of time messing up with pp, seems it's not a good choice.
'''
dataSet, features = readData2('part1')

ppservers = ()
if len(sys.argv) > 1:
    ncpus = int(sys.argv[1])
    # Creates jobserver with ncpus workers
    job_server = pp.Server(ncpus, ppservers=ppservers)
else:
    # Creates jobserver with automatically detected number of workers
    job_server = pp.Server(ppservers=ppservers)

print "Starting pp with", job_server.get_ncpus(), "workers"

start_time = time.time()

inputs = (4,4,4,4)

jobs = [(input, job_server.submit(train,(input,dataSet,features,), (createTree2,bag,randint,calcShannonEnt,splitDataSet,chooseBestFeatureToSplit,bagFeatures,majorityCnt,), ("datetime","tree","readfile","featureAnalize","random","math","operator",))) for input in inputs]

models = []

for input, job in jobs:
	models.extend(job())

print models
print "Time elapsed: ", time.time() - start_time, "s"
job_server.print_stats()

inputs2 = ('part1','part3','part5','part7',)
jobs2 = [(input, job_server.submit(predict,(input,(models,features),input+'_out'), (classify,rearrange,majorLabel,), ("datetime","tree","readfile","featureAnalize","random",))) for input in inputs2]

for input, job in jobs2:
	print job()

print "Time elapsed: ", time.time() - start_time, "s"
job_server.print_stats()
'''
