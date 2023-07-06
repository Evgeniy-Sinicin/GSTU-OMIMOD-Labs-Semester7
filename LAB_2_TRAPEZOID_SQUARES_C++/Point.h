#pragma once
#include <string>

using namespace std;

class Point
{
	private:
		int _x = 0;
		int _y = 0;

	public:
		Point();
		Point(int, int);
		void set(int _x, int _y);
		int get_x();
		int get_y();
		string to_string();
		bool operator == (Point _point);
		bool operator != (Point _point);
};

