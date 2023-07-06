#include <string>
#include "Point.h"

using namespace std;

Point::Point()
{
	Point::_x = 0;
	Point::_y = 0;
}

Point::Point(int _x = 0, int _y = 0)
{
	Point::_x = _x;
	Point::_y = _y;
}

void Point::set(int _x, int _y)
{
	Point::_x = _x;
	Point::_y = _y;
}

bool  Point::operator == (Point _point)
{
	if (_point.get_x() == Point::_x &&
		_point.get_y() == Point::_y)
	{
		return true;
	}

	return false;
}

bool  Point::operator != (Point _point)
{
	if (_point.get_x() == Point::_x &&
		_point.get_y() == Point::_y)
	{
		return false;
	}

	return true;
}

int Point::get_x()
{
	return Point::_x;
}

int Point::get_y()
{
	return Point::_y;
}

string Point::to_string()
{
	return std::to_string(_x) + " " + std::to_string(_y);
}