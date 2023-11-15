//
// Created by Toby Smedes on 10/27/23.
//

#include "PointList.h"
#include "point.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;
using std::getline;
using std::vector;

PointList::PointList(int size): _points(new Point*[size]), _size(size), _capacity(0) {

}
PointList::~PointList() {
    for(int i = 0; i < _capacity; ++i) {
        delete _points[i];
    }
}
PointList::PointList(const PointList& copyList){
    _size = copyList._size;
    _capacity = copyList._capacity;
    _points = new Point*[_size];
    for(int i = 0; i < _capacity; ++i){
        _points[i] = new Point(copyList._points[i]->GetX(), copyList._points[i]->GetY());
    }
}
PointList& PointList::operator=(const PointList& copyList) {
    if (this != &copyList) {
        delete[] _points;
        _points = new Point *[copyList._size];
        for (int i = 0; i < copyList._size; ++i) {
            _points[i] = copyList._points[i];
        }
        _size = copyList._size;
        _capacity = copyList._capacity;
    }
    return *this;
}
void PointList::Read(istream& input) {
    for(int i = _capacity; input && i < _size; i++, _capacity++) {
        _points[i] = new Point(0, 0);
        _points[i]->Read(input);
    }
}
void PointList::Write(ostream& output) {
    for (int i = 0; i < _size; ++i) {
        _points[i]->Write(output);
    }
}
bool PointList::Equals(Point* a, Point* b){
    if(a->GetX() == b->GetX() && a->GetY() == b->GetY()) return true;
    return false;
}
void PointList::Delete(Point* delPoint) {
    for (int i = 0; i < _capacity; ++i) {
        if(Equals(delPoint, _points[i])) {
            for (int j = i; j < _capacity - 1; ++j) {
                _points[j] = _points[j + 1];
            }
        }
    }
    _capacity--;
}
void PointList::ClosestPoints(Point& a, Point& b) {
    if(_size < 2) {
        cout << "Not enough points." << endl;
        return;
    }
    a = *_points[0];
    b = *_points[1];
    for (int i = 0; i < _size; ++i) {
        for (int j = i + 1; j < _size; ++j) {
            if(a.Distance(b) > _points[i]->Distance(*_points[j])) {
                a = *_points[i];
                b = *_points[j];
            }
        }
    }
    cout << "Shortest distance: " << a.Distance(b) << endl;
    cout << "Between points: ";
    a.Write(cout);
    cout << " and ";
    b.Write(cout);
    cout << endl;
}

void PointList::FarthestPoints(Point& a, Point& b) {
    if(_capacity < 2) return;
    a = *_points[0];
    b = *_points[1];
    //Finds min and max X value- if multiple, goes by min/max Y.
    for (int i = 0; i < _capacity; ++i) {
        if(_points[i]->GetX() < a.GetX()) a = *_points[i];
        if(abs(_points[i]->GetX() - a.GetX()) < 1e-10) {
            if(_points[i]->GetY() < a.GetY()) a = *_points[i];
        }
        if(_points[i]->GetX() > b.GetX()) b = *_points[i];
        if(abs(_points[i]->GetX() - b.GetX()) < 1e-10) {
            if(_points[i]->GetY() > b.GetY()) b = *_points[i];
        }
    }
}
//Cross product from AB to AC
double PointList::CrossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.GetX() - a.GetX()) * (c.GetY() - a.GetY()) - (b.GetY() - a.GetY()) * (c.GetX() - a.GetX());
}
string PointList::ToString() {
    string list = "{";
    for (int i = 0; i < _capacity; ++i) {
        if(i == _capacity - 1) {
            list += "[" + to_string(_points[i]->GetX()) + "," + to_string(_points[i]->GetY()) + "]}";
            break;
        }
        list += "[" + to_string(_points[i]->GetX()) + "," + to_string(_points[i]->GetY()) + "], ";
    }
    return list;
}

vector<Point> PointList::FindPointsConvexHull() {
    vector<Point> convexPoints;
    Point* minX = new Point(0,0);
    Point* maxX = new Point(0,0);
    FarthestPoints(*minX, *maxX);
    //Add initial points to convex hull points vector
    convexPoints.push_back(*minX);
    convexPoints.push_back(*maxX);
    PointList initialPointList(*this);
    PointList section1(initialPointList), section2(initialPointList);
    //Remove initial line points from both sets
    section1.Delete(minX);
    section1.Delete(maxX);
    section2.Delete(minX);
    section2.Delete(maxX);
    for (int i = 0; i < _capacity; ++i) {
        //Check for being part of the line
        if(Equals(initialPointList._points[i], minX) || Equals(maxX, initialPointList._points[i])){
            continue;
        }
        //Remove points on the line
        if(abs((CrossProduct(*minX, *maxX, *initialPointList._points[i]) - 0)) < 1e-6){
            section1.Delete(initialPointList._points[i]);
            section2.Delete(initialPointList._points[i]);
        }
        //Separate points on either side of line
        else if(CrossProduct(*minX, *maxX, *initialPointList._points[i]) < 0) {
            section1.Delete(initialPointList._points[i]);
        }
        else {
            section2.Delete(initialPointList._points[i]);
        }
    }
    //Find the farthest point from line
    Point* maxP = section1._points[0];
    for(int i = 0; i < section1._capacity; ++i) {
        if(section1._points[i]->DistanceToLine(*minX, *maxX) > maxP->DistanceToLine(*minX, *maxX)) {
            maxP = section1._points[i];
        }
    }
    //Add the farthest point to convex hull point vector
    convexPoints.push_back(*maxP);
    //Remove far point from section (will be part of hull)
    section1.Delete(maxP);
    //Call recursion from points
    ConvexHull(section1, minX, maxP, convexPoints);
    ConvexHull(section1, maxP, maxX, convexPoints);
    //Find the farthest point from other half of data set
    maxP = section2._points[0];
    for(int i = 0; i < section2._capacity; ++i) {
        if(section2._points[i]->DistanceToLine(*minX, *maxX) > maxP->DistanceToLine(*minX, *maxX)) {
            maxP = section2._points[i];
        }
    }
    //Add other far point to set
    convexPoints.push_back(*maxP);
    //Remove far point from section (will be part of hull)
    section2.Delete(maxP);
    //Recurse from other half
    ConvexHull(section2, maxP, minX, convexPoints);
    ConvexHull(section2, maxX, maxP, convexPoints);
    return convexPoints;
}
void PointList::ConvexHull(const PointList& section, Point* a, Point* b, vector<Point>& convexPoints) {
    PointList section1(section);
    for (int i = 0; i < section._capacity; ++i) {
        //Removes points on the line
        if(abs((CrossProduct(*a, *b, *section._points[i])) - 0) < 1e-6 && !Equals(section._points[i], a) && !Equals(b, section._points[i])){
            section1.Delete(section._points[i]);
        }
        //Remove points on inside of line
        else if(CrossProduct(*a, *b, *section._points[i]) < 0) {
            section1.Delete(section._points[i]);
        }
    }
    //Base cases for recursion: adds single point to vector
    if(section1._capacity == 1){
        convexPoints.push_back(*section1._points[0]);
        return;
    }
    if(section1._capacity == 0) return;
    //Find the farthest point from line on outside
    Point* maxP = section1._points[0];
    for(int i = 0; i < section1._capacity; ++i) {
        if(section1._points[i]->DistanceToLine(*a, *b) > maxP->DistanceToLine(*a, *b)) {
            maxP = section1._points[i];
        }
    }
    //Add far point to points vector
    convexPoints.push_back(*maxP);
    //Remove far point from section (will be part of hull)
    section1.Delete(maxP);
    //Recurse from new sections
    ConvexHull(section1, a, maxP, convexPoints);
    ConvexHull(section1, maxP, b, convexPoints);
}
