#include <string>
#include <vector>
#include "StringManager.h"

using namespace std;

vector<string> StringManager::split(string _text, string _delimiter)
{
    size_t _pos = _text.find(_delimiter);
    size_t _initialPos = 0;
    string _line;
    vector<string> _lines;

    // Decompose statement
    while (_pos != string::npos) {
        _lines.push_back(_text.substr(_initialPos, _pos - _initialPos));
        _initialPos = _pos + 1;

        _pos = _text.find(_delimiter, _initialPos);
    }

    // Add the last one
    string _last = _text.substr(_initialPos, min(_pos, _text.size()) - _initialPos + 1);

    if (!_last.empty())
    {
        _lines.push_back(_last);
    }

    return _lines;
}