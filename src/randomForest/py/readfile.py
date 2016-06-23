import sys
import featureAnalize
from featureAnalize import *

def splitData(fname):
	nline = 200000
	totalL = 2177020
	itern = totalL/nline
	f = open(fname)
	for j in range(itern):
		print j
		outf = open('data/part'+str(j), 'w')
		for i in range(nline):
			#outf.write(svmlibFormat(f.readline())+'\n')
			outf.write(f.readline())
		outf.close()
	f.close()
#formatted data for decision tree
def readData2(fname):
	f = open(fname)
	data = []
	print "reading data from "+fname
	featurec = myfilter(analyze(fname), 0.02)
	realFeatures = []
	for (u,v) in featurec:
		realFeatures.append(u)
	for line in f.readlines():
		if line != '':
			data.append(rearrange(line.strip(), realFeatures))
	f.close()
	return (data, realFeatures)

def output(fname, datalist):
	f = open(fname, 'w')
	for line in datalist:
		for i in range(len(line)):
			if i == 0:
				f.write(str(line[i]))
			else:
				f.write(' '+str(line[i]))
		f.write('\n')
	f.close()

def outputFormat(filename, outname):
	f = open(filename)
	ff = open(outname, 'w')
	ff.write("id,label\n")
	labels = f.readlines()
	for i in range(len(labels)):
		ff.write(str(i)+','+labels[i])
	f.close()
	ff.close()

