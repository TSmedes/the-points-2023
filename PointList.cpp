//
// Created by Toby Smedes on 10/27/23.
//

#include "PointList.h"
#include "point.h"
#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;
using std::getline;

PointList::PointList(int size): _points(new Point*[size]), _size(size), _capacity(0) {

}
PointList::~PointList() {
    for(int i = 0; i < _size; ++i) {
        delete _points[i];
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
void PointList::Delete(Point* delPoint) {
    /*for(int i = 0; i < _capacity; ++i) {
        if(abs(_points[i]->GetX() - x) < 1e-10 && abs(_points[i]->GetY() - y) < 1e-10) {
            for (int j = i; j < _capacity - 2; ++j) {
                _points[j] = _points[j + 1];
            }
        }
    }*/
    for (int i = 0; i < _capacity; ++i) {
        if(delPoint == _points[i]) {
            for (int j = i; j < _capacity - 2; ++j) {
                _points[j] = _points[j + 1];
            }
        }
    }
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
//Cross product from AB to AC
double PointList::CrossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.GetX() - a.GetX()) * (c.GetY() - a.GetY()) - (b.GetY() - a.GetY()) * (c.GetX() - a.GetY());
}
string PointList::ToString() {
    string list = "{";
    for (int i = 0; i < _size; ++i) {
        if(i == _size - 1) {
            list += "[" + to_string(_points[i]->GetX()) + "," + to_string(_points[i]->GetY()) + "]}";
            break;
        }
        list += "[" + to_string(_points[i]->GetX()) + "," + to_string(_points[i]->GetY()) + "], ";
    }
    return list;
}

double PointList::FindPerimeterConvexHull() {
    double perim = 0.0;
    Point* minX, *maxX;
    minX = _points[0];
    maxX = _points[1];
    //Finds min and max X value- if multiple, goes by min/max Y.
    for (int i = 0; i < _capacity; ++i) {
        if(_points[i]->GetX() < minX->GetX()) minX = _points[i];
        if(abs(_points[i]->GetX() - minX->GetX()) < 1e-10) {
            if(_points[i]->GetY() < minX->GetY()) minX = _points[i];
        }
        if(_points[i]->GetX() > maxX->GetX()) maxX = _points[i];
        if(abs(_points[i]->GetX() - maxX->GetX()) < 1e-10) {
            if(_points[i]->GetY() > maxX->GetY()) maxX = _points[i];
        }
    }
    ConvexHull(*this, minX, maxX);
}
Point PointList::ConvexHull(PointList section, Point* a, Point* b) {
    if(section._size == 1) return *section._points[0];
    //Check if this copies value or reference
    PointList section2 = section;
    for (int i = 0; i < _capacity; ++i) {
        if(CrossProduct(*a, *b, *section._points[i]) < 0) {
            section.Delete(section._points[i]);
            //FIXME this should come out of master loop
            Point* maxP = section._points[0];
            for(int j = 0; j < section._capacity; ++j) {
                if(section._points[j]->DistanceToLine(*a, *b) > maxP->DistanceToLine(*a, *b)) {
                    maxP = section._points[j];
                }
            }
            ConvexHull(section, a, maxP);
            ConvexHull(section, b, maxP);
            ///Fix recursion thing
        }
        else {
            section2.Delete(section2._points[i]);
        }
    }
}
//Paused at:
//recursion in ConvexHull()
//Just checked for which side of line from cross product
//Need to split up smaller PointList objects and call their own ConvexHull()

/// Do else condition recursing