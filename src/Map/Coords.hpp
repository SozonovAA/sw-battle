#pragma once

#include <string>
#include <ostream>

namespace sw::map {

struct Point
{
    unsigned _x;
    unsigned _y;
    
    Point() : _x(0), _y(0) {}
    Point(unsigned x_val, unsigned y_val) : _x(x_val), _y(y_val) {}
    
    friend std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << "Point: (" << point._x << ", " << point._y << ")";
        return os;
    }
};
    
} // namespace sw::map