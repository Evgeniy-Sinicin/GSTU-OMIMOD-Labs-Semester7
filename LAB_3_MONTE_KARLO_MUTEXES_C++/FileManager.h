#pragma once
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class FileManager
{
	public:
		static void writeToFile(string _message, string _path, bool _needToRecreate = false);
		static string readFromFile(string _path);
};