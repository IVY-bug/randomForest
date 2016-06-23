def analyze(fname):
	f_map = {}
	f = open(fname)
	data = f.readlines()[1:-1]
	for line in data:
		features = line.strip().split(' ')[1:]
		for feature in features:
			linen = feature.split(':')[0]
			if linen not in f_map:
				f_map[linen] = 0
			f_map[linen] += 1
	ret = sorted(f_map.items(), key=lambda x: x[1])
	return ret
	#for linen in f_map:
	#	print "{0}\t{1}\n".format(linen, f_map[linen])
def myfilter(data, th):
	total = 100000
	Min = th*total
	Max = total - Min
	ret = []
	for (linen, count) in data:
		if count > Min and count < Max:
			ret.append((linen,count))
	return ret
#retf = ret.keys(), important features
def rearrange(line, retf):
	tmp = line.strip().split(' ')
	label = line[0]
	line = []
	for item in tmp[1:]:
		line.append(item.split(':')[0])
	ret = []
	for i in range(len(retf)):
		feature = retf[i]
		if feature in line:
			ret.append(1)
		else:
			ret.append(0)
	ret.append(label)
	return ret



