"""
Part 3: Here you should improve viterbi to use better laplace smoothing for unseen words
This should do better than baseline and your first implementation of viterbi, especially on unseen words
"""
import math
from collections import OrderedDict
def viterbi_3(train, test):
    '''
    input:  training data (list of sentences, with tags on the words)
            test data (list of sentences, no tags on the words)
    output: list of sentences with tags on the words
            E.g., [[(word1, tag1), (word2, tag2)], [(word3, tag3), (word4, tag4)]]
    '''
    #difference: use hapax(word occur only once in training data)
    #when do lapalce of emission p for tag T, scale laplace cosntant by p of tag T occur in hapax
    
    # initial prob
    laplace_smoothing = 0.001
    
    three_char = {}
    two_char = {}
    word_in_train = {}
    for each_sentence in train:
        for pair in each_sentence:
            word = pair[0]
            # if len(word) >3:
            #     if word[-3:] not in three_char:
            #         three_char[word[-3:]] = 1
            #     else:
            #         three_char[word[-3:]] += 1
            # if len(word) > 2:
            #     if word[-2:] not in two_char:
            #         two_char[word[-2:]] = 1
            #     else:
            #         two_char[word[-2:]] += 1
            
            if word not in word_in_train:
                word_in_train[word] = 1
    #print(word_in_train.keys())
    #print(OrderedDict(sorted(three_char.items(), key=lambda t: t[1])))
    
    
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
    tag_collection = {}
    tag_occurence = {}
    
    for each_sentence in train:
        for pair in each_sentence:
            tag = pair[1]
            if tag in tag_occurence:
                tag_occurence[tag] += 1
            else:
                tag_collection[tag] = 1
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
    
    #word with suffix -ly, -ing, -able, -ment
    # -ly: many advers
    # -ing: most verb
    # -able: most adj
    # -ment: most noun
    
    word_count = {}
    unique_count = 0
    for each_sentence in train:
        for pair in each_sentence:
            word = pair[0]
            if word in word_count:
                word_count[word] += 1
            else:
                word_count[word] = 1
    #print(word_count)
    hapax_word = {}
        
    for each_word in word_count:  
        if word_count[each_word] == 1:
            hapax_word[each_word] = 1
    
    hapax_prob = {}
    
    ly = {}
    ing = {}
    es = {}
    ed = {}
    _s = {}
    ment = {}
    al = {}
    dom = {}
    ism = {}
    able = {}
    ish = {}
    ful = {}
    ive = {}
    ate = {}
    en = {}
    fy = {}
    s = {}
    se = {}
    er = {}
    on = {}
    ion = {}
    ent = {}
    ant = {}
    th = {}
    ity = {}
    ous = {}
    number = {}
    
    
    for each_tag in tag_collection.keys():
        hapax_prob[each_tag] = laplace_smoothing
        ly[each_tag] = laplace_smoothing
        ing[each_tag] = laplace_smoothing
        es[each_tag] = laplace_smoothing
        ed[each_tag] = laplace_smoothing
        _s[each_tag] = laplace_smoothing
        ment[each_tag] = laplace_smoothing
        al[each_tag] = laplace_smoothing
        dom[each_tag] = laplace_smoothing
        ism[each_tag] = laplace_smoothing
        able[each_tag] = laplace_smoothing
        ish[each_tag] = laplace_smoothing
        ful[each_tag] = laplace_smoothing
        ive[each_tag] = laplace_smoothing
        ate[each_tag] = laplace_smoothing
        en[each_tag] = laplace_smoothing
        fy[each_tag] = laplace_smoothing
        s[each_tag] = laplace_smoothing
        se[each_tag] = laplace_smoothing
        er[each_tag] = laplace_smoothing
        on[each_tag] = laplace_smoothing
        
        ion[each_tag] = laplace_smoothing
        ent[each_tag] = laplace_smoothing
        ant[each_tag] = laplace_smoothing
        th[each_tag] = laplace_smoothing
        ity[each_tag] = laplace_smoothing
        ous[each_tag] = laplace_smoothing
        number[each_tag] = laplace_smoothing
            
    for each_sentence in train:
        for pair in each_sentence:
            word = pair[0]
            tag = pair[1]
            if word in hapax_word.keys():
                hapax_prob[tag] += 1
            if word.endswith('ly'):
                ly[tag] += 1
            if word.endswith('ing'):
                ing[tag] += 1
            if word.endswith('es'):
                es[tag] += 1
            if word.endswith('ed'):
                ed[tag] += 1
            if word.endswith("ment"):
                ment[tag] += 1
            if word.endswith("al"):
                al[tag] += 1
            if word.endswith("dom"):
                dom[tag] += 1
            if word.endswith("ism"):
                ism[tag] += 1
            if word.endswith("able"):
                able[tag] += 1
            if word.endswith("ish"):
                ish[tag] += 1
            if word.endswith("ful"):
                ful[tag] += 1
            if word.endswith("ive"):
                ive[tag] += 1
            if word.endswith("ate"):
                ate[tag] += 1
            if word.endswith("en"):
                en[tag] += 1
            if word.endswith("fy"):
                fy[tag] += 1
            if word.endswith("se"):
                se[tag] += 1
            if word.endswith("er"):
                er[tag] += 1
            
            if word.endswith("ent"):
                ent[tag] += 1
            if word.endswith("ant"):
                ant[tag] += 1
            if word.endswith("th"):
                th[tag] += 1
            if word.endswith("ity"):
                ity[tag] += 1
            if word.endswith("ous"):
                ous[tag] += 1
            
            if word.endswith("ion"):
                ion[tag] += 1
            elif word.endswith("on"):
                on[tag] += 1
              
            if word.endswith("'s"):
                _s[tag] += 1
            elif word.endswith('s'):
                s[tag] += 1  
            
            if word[-1:].isdigit()==True:
                number[tag] += 1
    
    for each_tag in tag_collection.keys():
        hapax_prob[each_tag] /= (len(hapax_word.keys()))
        ly[each_tag] /= (len(hapax_word.keys()))
        ing[each_tag] /= (len(hapax_word.keys()))
        es[each_tag] /= (len(hapax_word.keys()))
        ed[each_tag] /= (len(hapax_word.keys()))
        _s[each_tag] /= (len(hapax_word.keys()))
        ment[each_tag] /= (len(hapax_word.keys()))
        al[each_tag] /= (len(hapax_word.keys()))
        dom[each_tag] /= (len(hapax_word.keys()))
        ism[each_tag] /= (len(hapax_word.keys()))
        able[each_tag] /= (len(hapax_word.keys()))
        ish[each_tag] /= (len(hapax_word.keys()))
        ful[each_tag] /= (len(hapax_word.keys()))
        ive[each_tag] /= (len(hapax_word.keys()))
        ate[each_tag] /= (len(hapax_word.keys()))
        en[each_tag] /= (len(hapax_word.keys()))
        fy[each_tag] /= (len(hapax_word.keys()))
        s[each_tag] /= (len(hapax_word.keys()))
        se[each_tag] /= (len(hapax_word.keys()))
        er[each_tag] /= (len(hapax_word.keys()))
        on[each_tag] /= (len(hapax_word.keys()))
        
        ion[each_tag] /= (len(hapax_word.keys()))
        ent[each_tag] /= (len(hapax_word.keys()))
        ant[each_tag] /= (len(hapax_word.keys()))
        th[each_tag] /= (len(hapax_word.keys()))
        ity[each_tag] /= (len(hapax_word.keys()))
        ous[each_tag] /= (len(hapax_word.keys()))
        number[each_tag] /= (len(hapax_word.keys()))
        

    hapax_prob['UNK'] = 1 / (len(hapax_word.keys()))

    #print(hapax_prob)
    
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
    emission_ly = {}
    emission_ing = {}
    emission_es = {}
    emission_ed = {}
    emission__s = {}
    emission_ment = {}
    emission_al = {}
    emission_dom = {}
    emission_ism = {}
    emission_able = {}
    emission_ish = {}
    emission_ful = {}
    emission_ive = {}
    emission_ate = {}
    emission_en = {}
    emission_fy = {}
    emission_s = {}
    emission_se = {}
    emission_er = {}
    emission_on = {}
    
    emission_ion = {}
    emission_ent = {}
    emission_ant = {}
    emission_th = {}
    emission_ity = {}
    emission_ous = {}
    emission_number = {}
    
    total_emission_count = sum(emission_count.values())
    for key in emission_count.keys():
        emission_prob[key] = (emission_count[key]+laplace_smoothing) / (tag_occurence[key[1]]+laplace_smoothing*(unique_count+1))
        if key[1] not in emission_prob:
            emission_prob[key[1]] = (laplace_smoothing*hapax_prob[key[1]]) / (tag_occurence[key[1]]+laplace_smoothing*(unique_count+1))
    emission_prob['UNK'] = (laplace_smoothing*hapax_prob['UNK']) / (tag_occurence[key[1]]+laplace_smoothing*(unique_count+1))
    for each_tag in tag_collection.keys():
        emission_ly[each_tag] = (laplace_smoothing*ly[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ing[each_tag] = (laplace_smoothing*ing[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_es[each_tag] = (laplace_smoothing*es[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ed[each_tag] = (laplace_smoothing*ed[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission__s[each_tag] = (laplace_smoothing*_s[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ment[each_tag] = (laplace_smoothing*ment[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_al[each_tag] = (laplace_smoothing*al[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_dom[each_tag] = (laplace_smoothing*dom[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ism[each_tag] = (laplace_smoothing*ism[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))

        emission_able[each_tag] = (laplace_smoothing*able[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ish[each_tag] = (laplace_smoothing*ish[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ful[each_tag] = (laplace_smoothing*ful[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ive[each_tag] = (laplace_smoothing*ive[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ate[each_tag] = (laplace_smoothing*ate[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_en[each_tag] = (laplace_smoothing*en[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_fy[each_tag] = (laplace_smoothing*fy[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_s[each_tag] = (laplace_smoothing*s[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        
        emission_se[each_tag] = (laplace_smoothing*se[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_er[each_tag] = (laplace_smoothing*er[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_on[each_tag] = (laplace_smoothing*on[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        
        emission_ion[each_tag] = (laplace_smoothing*ion[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ent[each_tag] = (laplace_smoothing*ent[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ant[each_tag] = (laplace_smoothing*ant[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_th[each_tag] = (laplace_smoothing*th[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ity[each_tag] = (laplace_smoothing*ity[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_ous[each_tag] = (laplace_smoothing*ous[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))
        emission_number[each_tag] = (laplace_smoothing*number[each_tag]) / (tag_occurence[each_tag]+laplace_smoothing*(unique_count+1))


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

        for y, tag in enumerate(tag_collection.keys()):
            for x, word in enumerate(each_sentence):
                if (word, tag) in emission_prob:
                    trellis[y][x].probability = emission_prob[(word, tag)]
                elif word.endswith('ly'):
                    trellis[y][x].probability = emission_ly[tag]
                elif word.endswith('ing'):
                    trellis[y][x].probability = emission_ing[tag]
                elif word.endswith('es'):
                    trellis[y][x].probability = emission_es[tag]
                elif word.endswith('ed'):
                    trellis[y][x].probability = emission_ed[tag]
                elif word.endswith("'s"):
                    trellis[y][x].probability = emission__s[tag]
                elif word.endswith('s'):
                    trellis[y][x].probability = emission_s[tag]
                elif word.endswith('ment'):
                    trellis[y][x].probability = emission_ment[tag]
                elif word.endswith('al'):
                    trellis[y][x].probability = emission_al[tag]
                elif word.endswith('dom'):
                    trellis[y][x].probability = emission_dom[tag]
                elif word.endswith('ism'):
                    trellis[y][x].probability = emission_ism[tag]
                elif word.endswith('able'):
                    trellis[y][x].probability = emission_able[tag]
                elif word.endswith('ish'):
                    trellis[y][x].probability = emission_ish[tag]
                elif word.endswith('ful'):
                    trellis[y][x].probability = emission_ful[tag]
                elif word.endswith('ive'):
                    trellis[y][x].probability = emission_ive[tag]
                elif word.endswith('ate'):
                    trellis[y][x].probability = emission_ate[tag]
                elif word.endswith('en'):
                    trellis[y][x].probability = emission_en[tag]
                elif word.endswith('fy'):
                    trellis[y][x].probability = emission_fy[tag]
                elif word.endswith('se'):
                    trellis[y][x].probability = emission_se[tag]
                elif word.endswith('er'):
                    trellis[y][x].probability = emission_er[tag]
                elif word.endswith('ion'):
                    trellis[y][x].probability = emission_ion[tag]
                elif word.endswith('on'):
                    trellis[y][x].probability = emission_on[tag]
                
                elif word.endswith('ent'):
                    trellis[y][x].probability = emission_ent[tag]
                elif word.endswith('ant'):
                    trellis[y][x].probability = emission_ant[tag]
                elif word.endswith('th'):
                    trellis[y][x].probability = emission_th[tag]
                elif word.endswith('ity'):
                    trellis[y][x].probability = emission_ity[tag]
                elif word.endswith('ous'):
                    trellis[y][x].probability = emission_ous[tag]
                elif word[-1:].isdigit()==True:
                    trellis[y][x].probability = emission_number[tag]
                
                elif tag in tag_collection.keys():
                    trellis[y][x].probability = emission_prob[tag]
                else:
                    trellis[y][x].probability = emission_prob['UNK']
                
                if x == 0:
                    trellis[y][x].previous_cell = 'BEGIN'
                trellis[y][x].pair = (word, tag)
                #print(trellis[y][x])
        
        for x, word in enumerate(each_sentence):
            for y, each_tag in enumerate(tag_collection.keys()):
                
                all_prob = {}
                maximum = -math.inf
                previous_maximum = 'BEGIN'
                maximum_index = ('None', 'None')
                #computer v * p_t * p_e for all tags
                for k, extra_tag in enumerate(tag_collection.keys()):
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
        for y in range(len(tag_collection.keys())):
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
