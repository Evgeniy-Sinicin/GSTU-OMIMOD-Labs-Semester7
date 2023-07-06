#pragma once
#include <string>
#include "Point.h"

using namespace std;

class Trapezoid
{
	private:
		Point _a;
		Point _b;
		Point _c;
		Point _d;
		int angle(Point _left, Point _middle, Point _right);

	public:
		Trapezoid();
		Trapezoid(Point _a, Point _b, Point _c, Point _d);
		int square();
		int get_angle_a();
		int get_angle_b();
		int get_angle_c();
		int get_angle_d();
		string to_string();
};