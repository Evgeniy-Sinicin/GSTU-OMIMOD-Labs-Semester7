#include "Math.h"
#include "Trapezoid.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

using namespace std;

Trapezoid::Trapezoid()
{
	Trapezoid::_a = Point();
	Trapezoid::_b = Point();
	Trapezoid::_c = Point();
	Trapezoid::_d = Point();
}

Trapezoid::Trapezoid(Point _a, Point _b, Point _c, Point _d)
{
	Trapezoid::_a = _a;
	Trapezoid::_b = _b;
	Trapezoid::_c = _c;
	Trapezoid::_d = _d;
}

int Trapezoid::square()
{
	int _down_width = _d.get_x() - _a.get_x();
	int _up_width = _c.get_x() - _b.get_x();
	int _height = _b.get_y() - _a.get_y();

	return ((_down_width + _up_width) / 2) * _height;
}

int Trapezoid::angle(Point _p0, Point _c, Point _p1)
{
	double _p1x = _c.get_x();
	double _p1y = _c.get_y();
	double _p2x = _p0.get_x();
	double _p2y = _p0.get_y();
	double _p3x = _p1.get_x();
	double _p3y = _p1.get_y();

	double _numerator = _p2y * (_p1x - _p3x) + _p1y * (_p3x - _p2x) + _p3y * (_p2x - _p1x);
	double _denominator = (_p2x - _p1x) * (_p1x - _p3x) + (_p2y - _p1y) * (_p1y - _p3y);
	double _ratio = _numerator / _denominator;

	double _angle_rad = atan(_ratio);
	double _angle_deg = (_angle_rad * 180) / 3.14159265358979323846;

	if (_angle_deg < 0) 
	{
		_angle_deg = 180 + _angle_deg;
	}

	return _angle_deg;
}

int Trapezoid::get_angle_a()
{
	return angle(Trapezoid::_b, Trapezoid::_a, Trapezoid::_d);
}

int Trapezoid::get_angle_b()
{
	return angle(Trapezoid::_c, Trapezoid::_b, Trapezoid::_a);
}

int Trapezoid::get_angle_c()
{
	return angle(Trapezoid::_d, Trapezoid::_c, Trapezoid::_b);
}

int Trapezoid::get_angle_d()
{
	return angle(Trapezoid::_a, Trapezoid::_d, Trapezoid::_c);
}

string Trapezoid::to_string()
{
	return "Point A: " + std::to_string(_a.get_x()) + ":" + std::to_string(_a.get_y()) + ";" +
		  " Point B: " + std::to_string(_b.get_x()) + ":" + std::to_string(_b.get_y()) + ";" +
		  " Point C: " + std::to_string(_c.get_x()) + ":" + std::to_string(_c.get_y()) + ";" +
		  " Point D: " + std::to_string(_d.get_x()) + ":" + std::to_string(_d.get_y()) + ";" +
		  " Angle A: " + std::to_string(get_angle_a()) + ";" +
		  " Angle B: " + std::to_string(get_angle_b()) + ";" +
		  " Angle C: " + std::to_string(get_angle_c()) + ";" +
		  " Angle D: " + std::to_string(get_angle_d()) + ";" +
		  " Square: " + std::to_string(square()) + ";";
}