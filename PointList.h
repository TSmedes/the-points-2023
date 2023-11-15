//
// Created by Toby Smedes on 10/27/23.
//

#ifndef THE_POINTS_2023_POINTLIST_H
#define THE_POINTS_2023_POINTLIST_H

#include "point.h"

#include <vector>
using std::vector;


class PointList {
private:
    Point** _points;
    int _size;
    int _capacity;

public:
    PointList(int size);
    ~PointList();
    PointList(const PointList& copyList);
    PointList& operator=(const PointList& copyList);
    void Read(istream& input);
    void Write(ostream& output);
    bool Equals(Point* a, Point* b);
    void Delete(Point* delPoint);
    void ClosestPoints(Point& a, Point& b);
    void FarthestPoints(Point& a, Point& b);
    double CrossProduct(const Point& a, const Point& b, const Point& c);
    string ToString();
    vector<Point> FindPointsConvexHull();
    void ConvexHull(const PointList& section, Point* a, Point* b, vector<Point>& convexPoints);
};


#endif //THE_POINTS_2023_POINTLIST_H
