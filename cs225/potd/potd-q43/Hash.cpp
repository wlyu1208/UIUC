#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Hash.h"
#include <map>
using namespace std;

unsigned long bernstein(std::string str, int M)
{
	unsigned long b_hash = 5381;
	for (int i = 0; i < (int) str.length(); ++i)
	{
		b_hash = b_hash * 33 + str[i];
	}
	return b_hash % M;
}

float hash_goodness(std::string str, int M)
{
    std::vector<int>* array = new std::vector<int>(M);	// Hint: This comes in handy
	int permutation_count = 0;
	float goodness = 0;
	map<unsigned long, int> result;
	do {
		if (permutation_count == M) break;
		// Code for computing the hash and updating the array
		unsigned long  x=bernstein(str, M);
		(*array)[permutation_count]=x;
		permutation_count++;
		result[x]++;

	} while(std::next_permutation(str.begin(), str.end()));

	//Code for determining goodness
	int i=0;
	for(auto&it:result){
		if(it.second!=1){
			i+=(it.second-1);
		}
	}
	goodness=(float)i/M;
	return goodness;
}
