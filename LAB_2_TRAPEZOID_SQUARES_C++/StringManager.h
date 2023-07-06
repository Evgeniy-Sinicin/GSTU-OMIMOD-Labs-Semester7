#pragma once
#include <string>
#include <vector>

using namespace std;

class StringManager
{
	public:
		static vector<string> split(string _text, string _delimiter);
};