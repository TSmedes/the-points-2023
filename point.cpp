//
// Created by Carlos R. Arias on 10/27/23.
//

#include "point.h"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

using std::string;
using std::stringstream;
using std::ostream;
using std::istream;

Point::Point(double x, double y): _x(x), _y(y) {

}

string Point::ToString() const {
    stringstream ss;
    ss << "[" << _x << ", " << _y << "]";
    return ss.str();
}

double Point::Distance(const Point &point) {
    return sqrt(pow(_x - point._x, 2.0) + pow(_y - point._y, 2.0));
}
double Point::DistanceToLine(const Point& a, const Point& b){
    double slope = (b.GetY() - a.GetY()) / (b.GetX() - a.GetX());
    double c = b.GetY() - slope * b.GetX();
    double distance = abs(slope * GetX() - GetY() + c);
    return distance / sqrt(slope * slope + 1);
}

double Point::GetX() const {
    return _x;
}

double Point::GetY() const {
    return _y;
}

void Point::Write(ostream &output) const {
    output << _x << " " << _y;
}

void Point::Read(istream &input) {
    input >> _x >> _y;
}
