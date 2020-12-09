# naive_bayes.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Justin Lizama (jlizama2@illinois.edu) on 09/28/2018

"""
This is the main entry point for MP3. You should only modify code
within this file and the last two arguments of line 34 in mp3.py
and if you want-- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.
"""
import collections
import math
import numpy

def naiveBayes(train_set, train_labels, dev_set, smoothing_parameter=1.0, pos_prior=0.8):
    """
    train_set - List of list of words corresponding with each movie review
    example: suppose I had two reviews 'like this movie' and 'i fall asleep' in my training set
    Then train_set := [['like','this','movie'], ['i','fall','asleep']]

    train_labels - List of labels corresponding with train_set
    example: Suppose I had two reviews, first one was positive and second one was negative.
    Then train_labels := [1, 0]

    dev_set - List of list of words corresponding with each review that we are testing on
              It follows the same format as train_set

    smoothing_parameter - The smoothing parameter --laplace (1.0 by default)
    pos_prior - The prior probability that a word is positive. You do not need to change this value.
    """
    # TODO: Write your code here
    # return predicted labels of development set
    #print("work?")
    #print(range(len(train_set)))
    #print(train_labels)
    #print(train_set)
    positive_count = {}
    negative_count = {}
    train_length = range(len(train_set))
    # 1:positive 0:negative in label
    for i in train_length:
        current_set = train_set[i]
        current_label = train_labels[i]

        for each in current_set:
            
            if current_label == 0:
                if each not in negative_count.keys():
                    negative_count[each] = 1
                else:
                    negative_count[each] += 1
            else:
                if each not in positive_count.keys():
                    positive_count[each] = 1
                else:
                    positive_count[each] += 1


    total_pcount = sum(positive_count.values())
    total_ncount = sum(negative_count.values())

    p_v = len(positive_count)
    p_n = len(negative_count)
    #print(p_v, p_n)
    final = []
    #P(UNK \ C) or P (W\C)
    #P(UNK \ C) = a / (n+a(V+1))
    #P(W \ C) = (count(W)+a) / (n+a(V+1))
    for line in dev_set:
        pos_prob = pos_prior
        neg_prob = 1-pos_prior
        for each in line:
            
            if each not in positive_count:
                probability = smoothing_parameter / (total_pcount+smoothing_parameter * (p_v+1))
            else:
                probability = (positive_count[each] + smoothing_parameter) / (total_pcount+smoothing_parameter * (p_v+1))
            
            pos_prob += math.log10(probability)

            if each not in negative_count:
                probability = smoothing_parameter / (total_ncount+smoothing_parameter * (p_n+1))
            else:
                probability = (negative_count[each] + smoothing_parameter) / (total_ncount+smoothing_parameter * (p_n+1))

            neg_prob += math.log10(probability)
        
        if pos_prob <= neg_prob:
            final.append(0)
        else:
            final.append(1)
    
    return final

def bigramBayes(train_set, train_labels, dev_set, unigram_smoothing_parameter=0.0125, bigram_smoothing_parameter=0.0125, bigram_lambda=0.5,pos_prior=0.8):
    """
    train_set - List of list of words corresponding with each movie review
    example: suppose I had two reviews 'like this movie' and 'i fall asleep' in my training set
    Then train_set := [['like','this','movie'], ['i','fall','asleep']]

    train_labels - List of labels corresponding with train_set
    example: Suppose I had two reviews, first one was positive and second one was negative.
    Then train_labels := [1, 0]

    dev_set - List of list of words corresponding with each review that we are testing on
              It follows the same format as train_set

    unigram_smoothing_parameter - The smoothing parameter for unigram model (same as above) --laplace (1.0 by default)
    bigram_smoothing_parameter - The smoothing parameter for bigram model (1.0 by default)
    bigram_lambda - Determines what fraction of your prediction is from the bigram model and what fraction is from the unigram model. Default is 0.5
    pos_prior - The prior probability that a word is positive. You do not need to change this value.
    """
    # TODO: Write your code here
    # return predicted labels of development set using a bigram model
    
    #print("work?")

    uni_positive = {}
    uni_negative = {}

    train_length = range(len(train_set))

    #print("start train uni")
    for i in train_length:
        current_set = train_set[i]
        current_label = train_labels[i]

        for each in current_set:
            if current_label == 0:
                if each not in uni_negative.keys():
                    uni_negative[each] = 1
                else:
                    uni_negative[each] += 1
            else:
                if each not in uni_positive.keys():
                    uni_positive[each] = 1
                else:
                    uni_positive[each] += 1

    uni_pos_count = sum(uni_positive.values())
    uni_neg_count = sum(uni_negative.values())
    uni_pos_v = len(uni_positive)
    uni_neg_v = len(uni_negative)
    #print("done train uni")

    bi_positive = {}
    bi_negative = {}

    for each in train_set:
        for j in range(len(each)-1):
            bi_word = each[j] + each[j+1]
            if train_labels[train_set.index(each)] == 0:
                if bi_word not in bi_negative:
                    bi_negative[bi_word] = 1
                else:
                    bi_negative[bi_word] += 1
            else:
                if bi_word not in bi_positive:
                    bi_positive[bi_word] = 1
                else:
                    bi_positive[bi_word] += 1
    
    bi_pos_count = sum(bi_positive.values())
    bi_neg_count = sum(bi_negative.values())
    bi_pos_v = len(bi_positive)
    bi_neg_v = len(bi_negative)
    #print("done train bi")
    #print(uni_pos_count, uni_neg_count, bi_pos_count, bi_pos_count)

    final = []
    for line in dev_set:
        pos_prob_uni = pos_prior
        neg_prob_uni = 1 - pos_prior
        pos_prob_bi = pos_prior
        neg_prob_bi = 1 - pos_prior

        for each in line:
            if each not in uni_positive:
                probability = unigram_smoothing_parameter / (uni_pos_count+ unigram_smoothing_parameter*(uni_pos_v+1))
            else:
                probability = (uni_positive[each] + unigram_smoothing_parameter) / (uni_pos_count+ unigram_smoothing_parameter*(uni_pos_v+1))
            
            pos_prob_uni += math.log10(probability)

            if each not in uni_negative:
                probability = unigram_smoothing_parameter / (uni_neg_count+ unigram_smoothing_parameter*(uni_neg_v+1))
            else:
                probability = (uni_negative[each] + unigram_smoothing_parameter) / (uni_neg_count+ unigram_smoothing_parameter*(uni_neg_v+1))
            
            neg_prob_uni += math.log10(probability)
        
        for index in range(len(line)-1):
            bi_word = line[index] + line[index+1]
            if bi_word not in bi_positive:
                probability = bigram_smoothing_parameter / (bi_pos_count+bigram_smoothing_parameter*(bi_pos_v+1))
            else:
                probability = (bi_positive[bi_word]+bigram_smoothing_parameter) / (bi_pos_count+bigram_smoothing_parameter*(bi_pos_v+1))

            pos_prob_bi += math.log10(probability)

            if bi_word not in bi_negative:
                probability = bigram_smoothing_parameter / (bi_neg_count+bigram_smoothing_parameter*(bi_neg_v+1))
            else:
                probability = (bi_negative[bi_word]+bigram_smoothing_parameter) / (bi_neg_count+bigram_smoothing_parameter*(bi_neg_v+1))

            neg_prob_bi += math.log10(probability)

        pos_uni_bi = (1-bigram_lambda) * pos_prob_uni + (bigram_lambda * +pos_prob_bi)
        neg_uni_bi = (1-bigram_lambda) * neg_prob_uni + (bigram_lambda * +neg_prob_bi)

        if pos_uni_bi <= neg_uni_bi:
            final.append(0)
        else:
            final.append(1)

    return final