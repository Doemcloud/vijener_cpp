#pragma once
#include <iostream>

class Point {
public:
    int x, y;
    bool isInfinity;

    Point(int x, int y) : x(x), y(y), isInfinity(false) {}
    Point() : x(0), y(0), isInfinity(true) {} // точка в бесконечности

    bool operator==(const Point& other) const {
        if (isInfinity && other.isInfinity) return true;
        if (isInfinity || other.isInfinity) return false;
        return x == other.x && y == other.y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        if (p.isInfinity) os << "Infinity";
        else os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};