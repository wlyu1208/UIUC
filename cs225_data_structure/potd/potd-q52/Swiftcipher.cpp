#include "Swiftcipher.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;


string decipherer(string filename) {
	string l;
	ifstream infile (filename);

	vector<string> v;

	if (infile.is_open()) {
			while (getline(infile, l)) {
					// do things
					v.push_back(l);
			}
	}

	map<string, int> mapping;
	for(unsigned i = 0; i < v.size(); i++){
		mapping[v[i]]++;
	}

	map<int, string> reverse;
	for(auto &it : mapping){
		reverse[it.second] = it.first;
	}


	string result = "";
	for(auto &it2 : reverse){
		result += it2.second;
		result += ' ';
	}
	result.pop_back();

	infile.close();
	return result;
}
