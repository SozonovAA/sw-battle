#pragma once
#include <string>
#include <ostream>
namespace sw::map {

struct Point {
    double x;
    double y;

    Point() : x(0.0), y(0.0) {}

    Point(double x_val, double y_val) : x(x_val), y(y_val) {}

    std::string print() const {
        std::string s;
        s.reserve(20);
        s.append("Point: (");
        s.append(std::to_string(x));
        s.append(", ");
        s.append(std::to_string(y));
        s.append(")");
        return s;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << "Point: (" << point.x << ", " << point.y << ")";
        return os;
    }
};
	
} // namespace sw::map