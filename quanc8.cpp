#include "quanc8.h"
#include <cmath>

void quanc8(double (*FUN)(double), double A, double B, double ABSERR,
            double RELERR, double *RESULT, double *ERREST, int *NOFUN, double *FLAG)

{
    double QRIGHT[32], F[17], X[17], FSAVE[9][31], XSAVE[9][31];
    int LEVMIN, LEVMAX, LEVOUT, NOMAX, NOFIN, LEV, NIM, J, I;
    double W0, W1, W2, W3, W4, COR11, AREA, X0, F0, STONE, STEP;
    double QLEFT, QNOW, QDIFF, QPREV, TOLERR, ESTERR;

    /*     ***  PASSO 1 *** INIZIALIZZAZIONE GENERALE COSTANTI */

    LEVMIN = 1;
    LEVMAX = 30;
    LEVOUT = 6;
    NOMAX = 5000;
    NOFIN = NOMAX - (8 * (LEVMAX - LEVOUT + pow(2, (double) (LEVOUT + 1))));

    /*     ISTRUZIONI RIGUARDO AL CASO IN CUI FUNCTION RAGGIUNGE IL VALORE NOFIN */

    W0 = 3956.0 / 14175.0;
    W1 = 23552.0 / 14175.0;
    W2 = -3712.0 / 14175.0;
    W3 = 41984.0 / 14175.0;
    W4 = -18160.0 / 14175.0;

    /*     INIZIALIZZO LE SOMME CORRENTI A ZERO */

    *FLAG = 0.0;
    *RESULT = 0.0;
    COR11 = 0.0;
    *ERREST = 0.0;
    AREA = 0.0;
    *NOFUN = 0;
    if (A == B)
        return;

    /*     ***  PASSO 2 ***  INIZIALIZZAZIONE RELATIVA AL PRIMO INTERVALLO */

    LEV = 0;
    NIM = 1;
    X0 = A;
    X[16] = B;
    QPREV = 0.0;
    F0 = FUN(X0);
    STONE = (B - A) / 16.0;
    X[8] = (X0 + X[16]) / 2.0;
    X[4] = (X0 + X[8]) / 2.0;
    X[12] = (X[8] + X[16]) / 2.0;
    X[2] = (X0 + X[4]) / 2.0;
    X[6] = (X[4] + X[8]) / 2.0;
    X[10] = (X[8] + X[12]) / 2.0;
    X[14] = (X[12] + X[16]) / 2.0;
    for (J = 2; J <= 16; J = J + 2) {
        F[J] = FUN(X[J]);
    }
    *NOFUN = 9;

    /*     ***  PASSO 3 ***  PARTE CENTRALE RIGUARDANTE I CALCOLI
    QUESTI CALCOLI IMPIEGANO I VALORI DI QPREV,X0,X2,X4,...X16,F0,F2,...F16.
    SI DETERMINANO I VALORI  X1,X3,...X15,F1,F3,...F15,QLEFT,QRIGTH,QNOW,
    QDIFF,AREA. */

    trenta:
    X[1] = (X0 + X[2]) / 2.0;
    F[1] = FUN(X[1]);
    for (J = 3; J <= 15; J = J + 2) {
        X[J] = (X[J - 1] + X[J + 1]) / 2.0;
        F[J] = FUN(X[J]);
    }
    *NOFUN = *NOFUN + 8;
    STEP = (X[16] - X0) / 16.0;
    QLEFT = (W0 * (F0 + F[8]) + W1 * (F[1] + F[7]) + W2 * (F[2] + F[6]) + W3 * (F[3] + F[5])
             + W4 * F[4]) * STEP;
    QRIGHT[LEV + 1] = (W0 * (F[8] + F[16]) + W1 * (F[9] + F[15]) + W2 * (F[10] + F[14])
                       + W3 * (F[11] + F[13]) + W4 * F[12]) * STEP;
    QNOW = QLEFT + QRIGHT[LEV + 1];
    QDIFF = QNOW - QPREV;
    AREA = AREA + QDIFF;

    /*     *** PASSO 4 ***  TEST SULLA CONVERGENZA */

    ESTERR = fabs(QDIFF) / 1023.0;
    if (ABSERR > (RELERR * fabs(AREA)) * (STEP / STONE))
        TOLERR = ABSERR;
    else
        TOLERR = (RELERR * fabs(AREA)) * (STEP / STONE);
    if (LEV < LEVMIN)
        goto cinquanta;
    if (LEV >= LEVMAX)
        goto sessantadue;
    if (*NOFUN > NOFIN)
        goto sessanta;
    if (ESTERR <= TOLERR)
        goto settanta;

    /*     *** PASSO 5 *** CASO DELLA NON CONVERGENZA */

    cinquanta:
    NIM = 2 * NIM;
    LEV = LEV + 1;

    /*  SI MEMORIZZA CIO' CHE PUO' SERVIRE PIU' AVANTI  */

    for (I = 1; I <= 8; I++) {
        FSAVE[I][LEV] = F[I + 8];
        XSAVE[I][LEV] = X[I + 8];
    }

    /*  SI ELABORA CIO' CHE SERVE ORA */

    QPREV = QLEFT;
    for (I = 1; I <= 8; I++) {
        J = -I;
        F[2 * J + 18] = F[J + 9];
        X[2 * J + 18] = X[J + 9];
    }
    goto trenta;

    /*     *** PASSO 6 ***  ISTRUZIONI RELATIVE AL CASO IN CUI  CI SONO DELLE
    DIFFICOLTA'.
    CIOE'  QUANDO IL NUMERO DEI VALORI DELLA FUNZIONE USATI PER
    DETERMINARE  RESULT SUPERA I LIMITI POSTI */

    sessanta:
    NOFIN = 2 * NOFIN;
    LEVMAX = LEVOUT;
    *FLAG = *FLAG + ((B - X0) / (B - A));
    goto settanta;

    /*    IL LIVELLO CORRENTE E' LEVMAX */

    sessantadue:
    *FLAG = *FLAG + 1.0;

    /*     *** PASSO 7 ***  CASO DELLA CONVERGENZA
    SOMMO I CONTRIBUTI TROVATI ALLE SOMME CORRENTI */

    settanta:
    *RESULT = *RESULT + QNOW;
    *ERREST = *ERREST + ESTERR;
    COR11 = COR11 + QDIFF / 1023.0;

    /*     DETERMINO IL PROSSIMO INTERVALLO */

    while (NIM % 2 != 0) {
        NIM = NIM / 2;
        LEV = LEV - 1;
    }
    NIM = NIM + 1;
    if (LEV <= 0)
        goto ottanta;

    /*     ELABORO GLI ELEMENTI  RELATIVI AL PROSSIMO INTERVALLO */

    QPREV = QRIGHT[LEV];
    X0 = X[16];
    F0 = F[16];
    for (I = 1; I <= 8; I++) {
        F[2 * I] = FSAVE[I][LEV];
        X[2 * I] = XSAVE[I][LEV];
    }
    goto trenta;

    /*     ***  PASSO 8 ***  DETERMINO GLI ULTIMI RISULTATI */

    ottanta:
    *RESULT = *RESULT + COR11;
    if (*ERREST == 0.0)
        return;
    while (fabs(*RESULT) + (*ERREST) == fabs(*RESULT))
        *ERREST = 2.0 * (*ERREST);
}