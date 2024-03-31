#pragma once
#include <string>
#include <ostream>
namespace sw::map {

struct Point {
    unsigned x_;
    unsigned y_;

    Point() : x_(0), y_(0) {}

    Point(unsigned x_val, unsigned y_val) : x_(x_val), y_(y_val) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << "Point: (" << point.x_ << ", " << point.y_ << ")";
        return os;
    }
};
	
} // namespace sw::map