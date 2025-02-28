#include "EllipticCurve.h"
#include <stdexcept>

Point EllipticCurve::addPoints(const Point& P, const Point& Q) const {
    if (P.isInfinity) return Q;
    if (Q.isInfinity) return P;
    if (P == Q) return doublePoint(P);
    if (P.x == Q.x) return Point(); // P + (-P) = бесконечность

    int numerator = (Q.y - P.y) % p;
    if (numerator < 0) numerator += p;
    int denominator = (Q.x - P.x) % p;
    if (denominator < 0) denominator += p;
    int lambda = (numerator * modInverse(denominator, p)) % p;

    int x3 = (lambda * lambda - P.x - Q.x) % p;
    if (x3 < 0) x3 += p;
    int y3 = (lambda * (P.x - x3) - P.y) % p;
    if (y3 < 0) y3 += p;

    return Point(x3, y3);
}

Point EllipticCurve::doublePoint(const Point& P) const {
    if (P.isInfinity) return Point();

    int numerator = (3 * P.x * P.x + a) % p;
    if (numerator < 0) numerator += p;
    int denominator = (2 * P.y) % p;
    if (denominator < 0) denominator += p;
    int lambda = (numerator * modInverse(denominator, p)) % p;

    int x3 = (lambda * lambda - 2 * P.x) % p;
    if (x3 < 0) x3 += p;
    int y3 = (lambda * (P.x - x3) - P.y) % p;
    if (y3 < 0) y3 += p;

    return Point(x3, y3);
}

Point EllipticCurve::scalarMultiply(int k, const Point& P) const {
    if (k == 0 || P.isInfinity) return Point();
    if (k < 0) throw std::invalid_argument("Скаляр k должен быть неотрицательным");
    int index = k % n;
    return points[index];
}

int EllipticCurve::modInverse(int a, int m) const {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
