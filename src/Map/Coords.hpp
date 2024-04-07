#pragma once

#include <string>
#include <ostream>

namespace sw::map {

struct Point
{
    using coord_type = int;
    coord_type _x;
    coord_type _y;
    
    Point() : _x(0), _y(0) {}
    Point(coord_type x_val, coord_type y_val) : _x(x_val), _y(y_val) {}

    Point operator+(const Point& other) const
    {
        return Point(_x + other._x, _y + other._y);
    }

    Point operator-(const Point& other) const
    {
        return Point(_x - other._x, _y - other._y);
    }

    bool operator==(const Point& other) const {
        return _x == other._x && _y == other._y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    
    friend std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << "Point: (" << point._x << ", " << point._y << ")";
        return os;
    }
};
    
} // namespace sw::map