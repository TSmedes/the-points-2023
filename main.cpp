#include "point.h"

#include <iostream>
#include <sstream>


using std::cout;
using std::endl;
using std::stringstream;


int main() {
    stringstream data(
            "5\n"
            "0 0\n"
            "1 4\n"
            "5 -1\n"
            "3 10\n"
            "4 -5"
            );
    int size;
    data >> size;
    Point** points;
    points = new Point*[size];

    for (int i = 0; i < 5; ++i) {
        points[i] = new Point(0,0);
        points[i]->Read(data);
        points[i]->Write(cout);
        cout << endl;
    }
    cout << points[0]->Distance(*points[4]) << endl;
    for (int i = 0; i < 5; ++i) {
        delete points[i];
    }
    delete[] points;
    return 0;
}
