#include "ECDHParticipant.h"

ECDHParticipant::ECDHParticipant(const EllipticCurve& curve) : curve(curve) {
    privateKey = generatePrivateKey();
    publicKey = curve.scalarMultiply(privateKey, curve.G);
}

Point ECDHParticipant::getPublicKey() const {
    return publicKey;
}

Point ECDHParticipant::computeSharedSecret(const Point& otherPublicKey) const {
    return curve.scalarMultiply(privateKey, otherPublicKey);
}

int ECDHParticipant::generatePrivateKey() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, curve.n - 1);
    return dis(gen);
}