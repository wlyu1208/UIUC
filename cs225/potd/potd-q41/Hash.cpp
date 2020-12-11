#include "Hash.h"
#include <string>

unsigned long bernstein(std::string str, int M)
{
	unsigned long b_hash = 5381;
	//Your code here
	char temp;
	for (unsigned i = 0; i < str.length(); i++) {
	temp = str[i];
	b_hash = (33 * b_hash) + temp;
}
	return b_hash % M;
}

std::string reverse(std::string str)
{
    std::string output = "";
	//Your code here
	int n = str.length();

	for (int i = 0; i < n/2; i++) {
		std::swap(str[i], str[n-i-1]);
	}
	output = str;
	return output;
}
