import math
import operator
import random
import datetime



def bagFeatures(labels):
    l = 15
    if len(labels) < l:
        return labels
    b = len(labels)
    ret = []
    for i in range(l):
        r = random.randint(0,b-1)
        ret.append(labels[r])
        del(labels[r])
        b -= 1
    return ret


def createTree(dataSet, labels, depth):
    classList = [example[-1] for example in dataSet]
    if depth > 2:
        return majorityCnt(classList)
    if len(dataSet[0]) == 1:
        return classList[0]
    if classList.count(classList[0]) == len(classList): 
        return classList[0]#stop splitting when all of the classes are equal
    if len(dataSet) == 1: #stop splitting when there are no more features in dataSet
        return majorityCnt(classList)
    labelcp = labels[:]
    mylabels = bagFeatures(labelcp)       #feature bagging
    bestFeatLabel,infoGain = chooseBestFeatureToSplit(dataSet, mylabels, labels)
    if infoGain == 0:
        return majorityCnt(classList)
    bestFeat = labels.index(bestFeatLabel)
    del(labels[bestFeat])
    myTree = {bestFeatLabel:{}}
    uniqueVals = set([0,1])
    for value in uniqueVals:
        sublabels = labels[:]
        subset = splitDataSet(dataSet, bestFeat, value)
        myTree[bestFeatLabel][value] = createTree(subset,sublabels, depth+1)
    return myTree
def createTree2(dataSet, labels, depth):
    print datetime.datetime.now()
    tree_labels = []
    tree_data = []
    d = []

    left_node = {}
    right_node = {}
    split_feature = {}
    label = {}
    
    #f_node = []
    tree_stack = []
    tid = 0

    tree_labels.append(labels[:])
    tree_data.append(dataSet)
    tree_stack.append(0)
    d.append(0)

    while len(tree_stack):
        cur_node = tree_stack[-1]
        del(tree_stack[-1])
        cur_dataSet = tree_data[cur_node]
        classList = [example[-1] for example in cur_dataSet]
        cur_labels = tree_labels[cur_node]
        cur_depth = d[cur_node]
        if cur_depth > depth:
            label[cur_node] = majorityCnt(classList)
            left_node[cur_node] = -1
            right_node[cur_node] = -1
            split_feature[cur_node] = -1
            continue
        if len(cur_dataSet[0]) == 1:
            label[cur_node] = majorityCnt(classList)
            left_node[cur_node] = -1
            right_node[cur_node] = -1
            split_feature[cur_node] = -1
            continue
        if classList.count(classList[0]) == len(classList): 
            label[cur_node] = majorityCnt(classList)
            left_node[cur_node] = -1
            right_node[cur_node] = -1
            split_feature[cur_node] = -1
            continue
        cur_labels = tree_labels[cur_node]
        baglabels = bagFeatures(cur_labels[:])
        best_feature, info_gain = chooseBestFeatureToSplit(cur_dataSet, baglabels, cur_labels)
        if info_gain == 0.0:
            label[cur_node] = majorityCnt(classList)
            left_node[cur_node] = -1
            right_node[cur_node] = -1
            split_feature[cur_node] = -1
            continue
        split_feature[cur_node] = best_feature
        best_feature_axis = cur_labels.index(best_feature)
        left_data = splitDataSet(cur_dataSet, best_feature_axis, 0)
        right_data = splitDataSet(cur_dataSet, best_feature_axis, 1)
        sublabels = cur_labels[:]
        del(sublabels[best_feature_axis])

        tree_labels.append(sublabels[:])
        tree_labels.append(sublabels[:])

        tree_data.append(left_data)
        tree_data.append(right_data)

        d.append(cur_depth+1)
        d.append(cur_depth+1)

        tid += 1
        left_node[cur_node] = tid
        tree_stack.append(tid+1)

        tid += 1
        right_node[cur_node] = tid
        tree_stack.append(tid-1)

    ret = {}
    cur = 0
    ret_stack = [0]
    p_stack = [ret]
    while len(ret_stack):
        cur_node = ret_stack[-1]
        cur_p = p_stack[-1]
        del(ret_stack[-1])
        del(p_stack[-1])

        cur_p[split_feature[cur_node]] = {0:{}, 1:{}}

        l_node = left_node[cur_node]
        r_node = right_node[cur_node]

        cur_p_l = cur_p[split_feature[cur_node]][0]
        cur_p_r = cur_p[split_feature[cur_node]][1]

        if split_feature[r_node] == -1:
            cur_p[split_feature[cur_node]][1] = label[r_node]
        else:
            ret_stack.append(r_node)
            p_stack.append(cur_p_r)
        if split_feature[l_node] == -1:
            cur_p[split_feature[cur_node]][0] = label[l_node]
        else:
            ret_stack.append(l_node)
            p_stack.append(cur_p_l)
    print datetime.datetime.now()
    return ret



def chooseBestFeatureToSplit(dataSet, labels, allLabels):
    numFeatures = len(dataSet[0]) - 1      #the last column is used for the labels
    baseEntropy = calcShannonEnt(dataSet)
    bestInfoGain = 0.0; bestFeature = -1
    for i in range(len(labels)):        #iterate over all the features
        uniqueVals = set([0,1])       #get a set of unique values
        newEntropy = 0.0
        axis = allLabels.index(labels[i])
        for value in uniqueVals:
            subDataSet = splitDataSet(dataSet, axis, value)
            prob = len(subDataSet)/float(len(dataSet))
            newEntropy += prob * calcShannonEnt(subDataSet)     
        infoGain = baseEntropy - newEntropy     #calculate the info gain; ie reduction in entropy
        if (infoGain > bestInfoGain):       #compare this to the best gain so far
            bestInfoGain = infoGain         #if better than current best, set to best
            bestFeature = i
    return (labels[bestFeature],bestInfoGain)                      #returns an integer

def splitDataSet(dataSet, axis, value):
    retDataSet = []
    for featVec in dataSet:
        if featVec[axis] == value:
            reducedFeatVec = featVec[:axis]     #chop out axis used for splitting
            reducedFeatVec.extend(featVec[axis+1:])
            retDataSet.append(reducedFeatVec)
    return retDataSet

def majorityCnt(classList):
    classCount={}
    for vote in classList:
        if vote not in classCount.keys(): classCount[vote] = 0
        classCount[vote] += 1
    sortedClassCount = sorted(classCount.iteritems(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]


def calcShannonEnt(dataSet):
    numEntries = len(dataSet)
    labelCounts = {}
    for featVec in dataSet: #the the number of unique elements and their occurance
        currentLabel = featVec[-1]
        if currentLabel not in labelCounts.keys(): labelCounts[currentLabel] = 0
        labelCounts[currentLabel] += 1
    shannonEnt = 0.0
    for key in labelCounts:
        prob = float(labelCounts[key])/numEntries
        shannonEnt -= prob * math.log(prob,2) #log base 2
    return shannonEnt


def classify(inputTree,testVec):
    if isinstance(inputTree, dict):
        classLabel = classify(inputTree[inputTree.keys()[0]][testVec[inputTree.keys()[0]]], testVec)
    else:
        classLabel = inputTree
    return classLabel


