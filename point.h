//
// Created by Carlos R. Arias on 10/27/23.
//

#ifndef THE_POINTS_2023_POINT_H
#define THE_POINTS_2023_POINT_H

#include <string>
#include <iostream>
using std::string;
using std::ostream;
using std::istream;

class Point {
private:
    double _x;
    double _y;
public:
    Point(double x , double y );
    string ToString()const;
    double Distance(const Point& point);
    double DistanceToLine(const Point& a, const Point& b);
    double GetX()const;
    double GetY()const;
    void Write(ostream& output)const;
    void Read(istream& input);

};


#endif //THE_POINTS_2023_POINT_H
