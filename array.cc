#include "array.h"

#include <cassert>
#include <iostream>

using std::endl;

Array::Array(int width, int height) : width(width), height(height), size(width*height) {
    data = new bool[size];
    for (int k=0; k<size; k++) { data[k] = false; }
}

Array::~Array() {
    delete [] data;
}

bool Array::parse(std::istream &is) {
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            char foo;
            is >> foo >> std::ws;
            if (is.bad()) return false;
            switch(foo) {
                case '0':
                    set_value(i,j,false);
                    break;
                case '1':
                    set_value(i,j,true);
                    break;
                default:
                    return false;
                    break;
            }
        }
    }
    return true;
}

int Array::get_number_of_ones() const {
    int count = 0;
    for (int k=0; k<size; k++) if (data[k]) count++;
    return count;
}

bool Array::get_value(int i, int j) const {
    assert(i >= 0 and i < height);
    assert(j >= 0 and j < width);
    return data[i+height*j];
}

void Array::set_value(int i, int j, bool value) {
    assert(i >= 0 and i < height);
    assert(j >= 0 and j < width);
    data[i+height*j] = value;
}


std::ostream &operator<<(std::ostream &os,const Array &array) {
    os << " ";
    for (int j=0; j<array.width; j++) { os << static_cast<char>('a'+j); }
    os << endl;

    for (int i=0; i<array.height; i++) {
        os << static_cast<char>('0'+i);
        for (int j=0; j<array.width; j++) { os << array.get_value(i,j); }
        os << endl;
    }

    return os;
}

