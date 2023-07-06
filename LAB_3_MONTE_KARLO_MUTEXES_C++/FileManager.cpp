#include <iostream>
#include <fstream>
#include <string>
#include "FileManager.h"

using namespace std;

void FileManager::writeToFile(string _message, string _path, bool _needToRecreate)
{
	ofstream _stream;

	if (_needToRecreate)
	{
		_stream.open(_path);
	}
	else
	{
		_stream.open(_path, ios::app);
	}

	if (_stream.is_open())
	{
		_stream << _message;

		_stream.close();
	}
}

string FileManager::readFromFile(string _path)
{
	string _data;
	ifstream _stream(_path);

	if (_stream.is_open())
	{
		string _line;

		while (getline(_stream, _line))
		{
			_data += _line + '\n';
		}
	}

	_stream.close();

	return _data;
}