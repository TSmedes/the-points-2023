#include "point.h"
#include "PointList.h"

#include <iostream>
#include <sstream>


using std::cout;
using std::endl;
using std::stringstream;

void PrintClosestPoints(Point** points, size_t size);

int main() {
    stringstream data(
            "5 12\n"
            "0 0\n"
            "1 4\n"
            "5 -1\n"
            "3 10\n"
            "4 -5"
            );
    int size = 6;
    PointList myPoints(size);
    myPoints.Read(data);
    cout << myPoints.ToString() << endl;
    Point a(0,0), b(0,0);
    myPoints.ClosestPoints(a, b);
    myPoints.FindPermiterConvexHull();
    return 0;
}
void PrintClosestPoints(Point** points, size_t size) {
    Point* point1;
    Point* point2;
    if(size < 2) {
        cout << "Not enough points." << endl;
        return;
    }
    point1 = points[0];
    point2 = points[1];
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if(point1->Distance(*point2) > points[i]->Distance(*points[j])) {
                point1 = points[i];
                point2 = points[j];
            }
        }
    }
    cout << "Shortest distance: " << point1->Distance(*point2) << endl;
    cout << "Between points: ";
    point1->Write(cout);
    cout << " and ";
    point2->Write(cout);
    cout << endl;
}