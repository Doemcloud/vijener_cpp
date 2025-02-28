#pragma once
#include "EllipticCurve.h"
#include <random>

class ECDHParticipant {
public:
    ECDHParticipant(const EllipticCurve& curve);

    Point getPublicKey() const;
    Point computeSharedSecret(const Point& otherPublicKey) const;

private:
    const EllipticCurve& curve;
    int privateKey;
    Point publicKey;

    int generatePrivateKey();
};