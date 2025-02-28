#pragma once
#include "Point.h"
#include <vector>

class EllipticCurve {
public:
    int a, b, p;
    Point G;
    int n;
    std::vector<Point> points;

    EllipticCurve(int a, int b, int p, Point G, int n)
        : a(a), b(b), p(p), G(G), n(n) {
        points.push_back(Point()); // 0*G = бесконечность
        Point current = G;
        for (int i = 1; i < n; ++i) {
            points.push_back(current);
            current = addPoints(current, G);
        }
        points.push_back(Point()); // n*G = бесконечность
    }

    Point addPoints(const Point& P, const Point& Q) const;
    Point doublePoint(const Point& P) const;
    Point scalarMultiply(int k, const Point& P) const;

private:
    int modInverse(int a, int m) const;
};