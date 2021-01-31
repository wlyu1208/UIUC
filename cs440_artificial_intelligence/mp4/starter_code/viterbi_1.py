"""
Part 2: This is the simplest version of viterbi that doesn't do anything special for unseen words
but it should do better than the baseline at words with multiple tags (because now you're using context
to predict the tag).
"""
import math

def viterbi_1(train, test):
    '''
    input:  training data (list of sentences, with tags on the words)
            test data (list of sentences, no tags on the words)
    output: list of sentences with tags on the words
            E.g., [[(word1, tag1), (word2, tag2)], [(word3, tag3), (word4, tag4)]]
    '''
    
    """
    initial p: how often does each tag occur at start of sentence
    transition p: how often does tag t_b follow tag t_a
    emission p: how often does tag t yield word w
    """
    
    # initial prob
    laplace_smoothing = 0.0001
    
    initial_count = {}
    unique_count = 0
    for each_sentence in train:
        start_tag = each_sentence[0][1]
        #print(each_sentence)
        #print(start_tag)
        if start_tag in initial_count:
            initial_count[start_tag] += 1
        else:
            initial_count[start_tag] = 1
            unique_count += 1
    
    #P(UNK \ C) = a / (n+a(V+1))
    #P(W \ C) = (count(W)+a) / (n+a(V+1))
    # laplace smooth = (count + laplace) / (total_count + laplace *(len(count)+1))
    initial_prob = {}
    total_initial_count = sum(initial_count.values())
    for key in initial_count.keys():
        initial_prob[key] = (initial_count[key] + laplace_smoothing) / (total_initial_count+laplace_smoothing*(unique_count+1))
    initial_prob['UNK'] = laplace_smoothing / (total_initial_count+laplace_smoothing*(unique_count+1))
    
    
    
    #before tag need number of appearance for each tag
    tag_collection = []
    tag_occurence = {}
    
    for each_sentence in train:
        for pair in each_sentence:
            tag = pair[1]
            if tag in tag_occurence:
                tag_occurence[tag] += 1
            else:
                tag_collection.append(tag)
                tag_occurence[tag] = 1
    
    #print(tag_occurence)
    #print(tag_collection)
    
    
    
    #transition prob
    transition_count = {}
    unique_count = 0
    for each_sentence in train:
        words = len(each_sentence)
        for each_index in range(1, words):
            current_tag = each_sentence[each_index][1]
            previous_tag = each_sentence[each_index-1][1]
            follow_tag = (current_tag, previous_tag)
            if follow_tag in transition_count:
                transition_count[follow_tag] += 1
            else:
                transition_count[follow_tag] = 1
                unique_count += 1
    
    transition_prob = {}
    #print(transition_count)
    #print(total_trans_count)
    total_transition_count = sum(transition_count.values())
    for key in transition_count.keys():
        transition_prob[key] = (transition_count[key]+laplace_smoothing) / (tag_occurence[key[1]]+laplace_smoothing*(unique_count+1))
    transition_prob['UNK'] = laplace_smoothing / (total_transition_count+laplace_smoothing*(unique_count+1))    
    
    
    
    #emission prob
    emission_count = {}
    unique_count = 0
    for each_sentence in train:
        for pair in each_sentence:
            if pair in emission_count:
                emission_count[pair] += 1
            else:
                emission_count[pair] = 1
                unique_count += 1
    
    emission_prob = {}
    total_emission_count = sum(emission_count.values())
    for key in emission_count.keys():
        emission_prob[key] = (emission_count[key]+laplace_smoothing) / (tag_occurence[key[1]]+laplace_smoothing*(unique_count+1))
        if key[1] not in emission_prob:
            emission_prob[key[1]] = laplace_smoothing / (tag_occurence[key[1]]+laplace_smoothing*(unique_count+1))
    emission_prob['UNK'] = laplace_smoothing / (total_emission_count+laplace_smoothing*(unique_count+1))
    
    #print(tag_collection)
    #print(emission_prob)
    #print(sum(initial_prob.values()))
    #print(sum(transition_prob.values()))
    #print(sum(emission_prob.values()))  
    #print(initial_count)
    #print(initial_prob)
    
    predict = []
    # n X m: n length of input, m num of unique tag
    for each_sentence in test:
        trellis = [[cell(0,'None', ('None', 'None')) for x in range(len(each_sentence))]for y in range(len(tag_collection))]

        for y, tag in enumerate(tag_collection):
            for x, word in enumerate(each_sentence):
                if (word, tag) in emission_prob:
                    trellis[y][x].probability = emission_prob[(word, tag)]
                elif tag in emission_prob:
                    trellis[y][x].probability = emission_prob[tag]
                else:
                    trellis[y][x].probability = emission_prob['UNK']
                if x == 0:
                    trellis[y][x].previous_cell = 'BEGIN'
                trellis[y][x].pair = (word, tag)
                #print(trellis[y][x])
        
        for x, word in enumerate(each_sentence):
            for y, each_tag in enumerate(tag_collection):
                
                all_prob = {}
                maximum = -math.inf
                previous_maximum = 'BEGIN'
                maximum_index = ('None', 'None')
                #computer v * p_t * p_e for all tags
                for k, extra_tag in enumerate(tag_collection):
                    if x > 0:
                        curr_pair = (each_tag, extra_tag)
                        if curr_pair not in transition_prob:
                            v = transition_prob['UNK']
                        else:
                            v = transition_prob[curr_pair]
                        
                        all_prob[extra_tag] = trellis[k][x-1].probability + math.log10(v) + math.log10(trellis[y][x].probability)
                    
                    else:
                        if each_tag not in initial_prob:
                            v = initial_prob['UNK']
                        else:
                            v = initial_prob[each_tag]
                        
                        all_prob[extra_tag] = math.log10(v) + math.log10(trellis[y][x].probability)
                
                    if all_prob[extra_tag] > maximum:
                        maximum = all_prob[extra_tag]
                        maximum_index = extra_tag
                        if x > 0:
                            previous_maximum = trellis[k][x-1]
                #print(current)
                trellis[y][x].probability = all_prob[maximum_index]
                trellis[y][x].previous_cell = previous_maximum
        
        maximum = -math.inf
        last_index = len(each_sentence)-1
        for y in range(len(tag_collection)):
            if trellis[y][last_index].probability > maximum:
                maximum = trellis[y][last_index].probability
                max_cell = trellis[y][last_index]
        
        end = max_cell
        best_path = []
        best_path.append(end.pair)
        while(end.previous_cell != 'BEGIN'):
            end = end.previous_cell
            best_path.append(end.pair)
        #print(result)   
        
        best_path.reverse()
        predict.append(best_path)
                
    return predict

class cell:
    def __init__(self, probability, previous_cell, pair):
        self.probability = probability
        self.previous_cell = previous_cell
        self.pair = pair
    
    def __str__(self):
        return "p: %s, pair: %s, p_tag: %s" % (str(self.probability), str(self.pair), str(self.previous_cell))
