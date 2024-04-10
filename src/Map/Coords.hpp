#pragma once

#include <string>
#include <ostream>

namespace sw::map {

struct Point
{
    using coord_type = int;
    coord_type x;
    coord_type y;
    
    Point() : x(0), y(0) {}
    Point(coord_type x_val, coord_type y_val) : x(x_val), y(y_val) {}

    Point operator+(const Point& other) const
    {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const
    {
        return Point(x - other.x, y - other.y);
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    
    friend std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << "Point: (" << point.x << ", " << point.y << ")";
        return os;
    }
};
    
} // namespace sw::map