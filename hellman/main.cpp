#include "EllipticCurve.h"
#include "ECDHParticipant.h"
#include <iostream>

using namespace std;

int main() {
    int a = 2;
    int b = 2;
    int p = 17;
    Point G(5, 1);
    int n = 19;

    EllipticCurve curve(a, b, p, G, n);

    ECDHParticipant alice(curve);
    ECDHParticipant bob(curve);

    Point alicePublic = alice.getPublicKey();
    Point bobPublic = bob.getPublicKey();

    Point aliceShared = alice.computeSharedSecret(bobPublic);
    Point bobShared = bob.computeSharedSecret(alicePublic);

    cout << "Открытый ключ Алисы: " << alicePublic << endl;
    cout << "Открытый ключ Боба: " << bobPublic << endl;
    cout << "Секрет Алисы: " << aliceShared << endl;
    cout << "Секрет Боба: " << bobShared << endl;

    if (aliceShared == bobShared) {
        cout << "Общие секреты совпадают: " << aliceShared << endl;
    } else {
        cout << "Общие секреты не совпадают!" << endl;
    }

    return 0;
}