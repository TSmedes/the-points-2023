#include "point.h"
#include "PointList.h"

#include <iostream>
#include <sstream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;
using std::stringstream;

int main() {
    stringstream data(
            "5 12\n"
            "0 0\n"
            "1 4\n"
            "5 -1\n"
            "3 10\n"
            "4 -5\n"
            "-3 0\n"
            "7 5\n"
            "3 6\n"
            "-1 14\n"
            "-2 6\n"
            "7 3"
            );
    int size = 12;
    PointList myPoints(size);
    myPoints.Read(data);
    cout << myPoints.ToString() << endl;
    vector<Point> convex = myPoints.FindPointsConvexHull();
    for(int i = 0; i < convex.size(); ++i){
        convex.at(i).Write(cout);
        cout << endl;
    }
    return 0;
}
