"""
Part 1: Simple baseline that only uses word statistics to predict tags
"""
"""
each word independent, ignore previous word and tage
each word, count w occr with tag in train
test data: give w the tag that was seen, unseen(guess tag that seen most often in train)
get about 93.9 accuracy on vrown corpus develop set
90% on multitag word, 69 on unseen
"""

import time


def baseline(train, test):
    '''
    input:  training data (list of sentences, with tags on the words)
            test data (list of sentences, no tags on the words)
    output: list of sentences, each sentence is a list of (word,tag) pairs.
            E.g., [[(word1, tag1), (word2, tag2)], [(word3, tag3), (word4, tag4)]]
    '''
    #print("work?")
    start_time = time.time()
    tag_count = {}
    for each_sentence in train:
        for pair in each_sentence:
            word = pair[0]
            tag = pair[1]
            
            if tag in tag_count:
                tag_count[tag] += 1
            else:
                tag_count[tag] = 1
    
    word_by_tag = {}
    for each_sentence in train:
        for pair in each_sentence:
            word = pair[0]
            tag = pair[1]
            
            if word in word_by_tag:
                data_tag = word_by_tag.get(word)
                #print(word, tag, data_tag)
                if tag in data_tag:
                    data_tag[tag] += 1
                else:
                    data_tag[tag] = 1
            else:
                word_by_tag[word] = {tag: 1}
    
    #print(tag_count)
    #print(word_by_tag)
    dict_val = list(tag_count.values())
    dict_key = list(tag_count.keys())
    most_tag = dict_key[dict_val.index(max(dict_val))]
    
    result= []
    for each_sentence in test:
        each_result = []
        for word in each_sentence:
            if word not in word_by_tag:
                each_result.append((word, most_tag))
            else:
                word_tag = word_by_tag[word]
                dict_val = list(word_tag.values())
                dict_key = list(word_tag.keys())
                highest_tag = dict_key[dict_val.index(max(dict_val))]
                each_result.append((word, highest_tag))
        result.append(each_result)
    
    #print(word_by_tag)        
    #print(tag_count)
    #print(most_tag)
    #print(test)
    end_time = time.time()
    print(end_time-start_time)
    return result



