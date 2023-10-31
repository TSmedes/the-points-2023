//
// Created by Toby Smedes on 10/27/23.
//

#ifndef THE_POINTS_2023_POINTLIST_H
#define THE_POINTS_2023_POINTLIST_H

#include "point.h"


class PointList {
private:
    Point** _points;
    int _size;
    int _capacity;

public:
    PointList(int size);
    ~PointList();
    PointList& operator=(const PointList& copyList);
    void Read(istream& input);
    void Write(ostream& output);
    void Delete(Point* delPoint);
    void ClosestPoints(Point& a, Point& b);
    double CrossProduct(const Point& a, const Point& b, const Point& c);
    string ToString();
    double FindPerimeterConvexHull();
    Point ConvexHull(PointList section, Point* a, Point* b);
};


#endif //THE_POINTS_2023_POINTLIST_H
