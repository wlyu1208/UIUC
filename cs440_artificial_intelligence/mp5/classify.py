# classify.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Justin Lizama (jlizama2@illinois.edu) on 10/27/2018
# Extended by Daniel Gonzales (dsgonza2@illinois.edu) on 3/11/2020

"""
This is the main entry point for MP5. You should only modify code
within this file -- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.

train_set - A Numpy array of 32x32x3 images of shape [7500, 3072].
            This can be thought of as a list of 7500 vectors that are each
            3072 dimensional.  We have 3072 dimensions because there are
            each image is 32x32 and we have 3 color channels.
            So 32*32*3 = 3072. RGB values have been scaled to range 0-1.

train_labels - List of labels corresponding with images in train_set
example: Suppose I had two images [X1,X2] where X1 and X2 are 3072 dimensional vectors
         and X1 is a picture of a dog and X2 is a picture of an airplane.
         Then train_labels := [1,0] because X1 contains a picture of an animal
         and X2 contains no animals in the picture.

dev_set - A Numpy array of 32x32x3 images of shape [2500, 3072].
          It is the same format as train_set

return - a list containing predicted labels for dev_set
"""

import numpy as np
from collections import Counter
import math

def trainPerceptron(train_set, train_labels, learning_rate, max_iter):
    # TODO: Write your code here
    # return the trained weight and bias parameters
    #print("train")
    
    #initialize weight and bias
    W = np.zeros(len(train_set[0]))
    b = 0
    
    for i in range(max_iter):
        for j, label in enumerate(train_labels):
            #print(j, label)
                if label == False:
                    label_num = -1
                else:
                    label_num = 1
            
            good_weight = np.sign(np.dot(W, train_set[j])+b)
            #print(good_weight)
            if label_num != good_weight:
                    
                W = W + learning_rate * label_num * train_set[j]
                b = b + learning_rate * label_num

    #print("train done")
    return W, b

def classifyPerceptron(train_set, train_labels, dev_set, learning_rate, max_iter):
    # TODO: Write your code here
    # Train perceptron model and return predicted labels of development set
    #print("classify")
    W, b = trainPerceptron(train_set, train_labels, learning_rate, max_iter)
    final = []
    for each_dev in dev_set:
        result = predict(W, each_dev, b)
        final.append(result)
    return final

def predict(W, x, b):
    good_weight = np.sign(np.dot(W, x)+b)
    if good_weight >= 0:
        result = 1
    else:
        result = 0
    return result

def classifyKNN(train_set, train_labels, dev_set, k):
    # TODO: Write your code here
    #print("KNN start")
    
    final = []
    for each_dev in dev_set:
        dist = []
        neighbor = []
        for i, label in enumerate(train_labels):
            #print(each_dev)
            #print(train_set[i])
            dist_rows = euc_dist(train_set[i], each_dev)
            dist.append((dist_rows, label))
        
        dist.sort(key=lambda x: x[0])
        
        for i in range(k):
            neighbor.append(dist[i][1])
        #print(neighbor)
        most_common = max(set(neighbor), key=neighbor.count)
        #print(most_common)
        final.append(int(most_common))

    return final

def euc_dist(row_1, row_2):
    distance = np.linalg.norm(row_1 - row_2)
    return distance