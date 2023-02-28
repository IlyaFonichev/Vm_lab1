#include "lagrange.h"

double lagrange(int n, const double *x, const double *y, double xRas) {
    int i = 0, j = 0;
    double p = 1, l = 0;
    for (i = 0; i <= n; i++) {
        for (p = 1, j = 0; j <= n; j++) {
            if (j != i)
                p = p * (xRas - (*(x + j))) / ((*(x + i)) - (*(x + j)));
        }
        l += (*(y + i)) * p;
    }
    return l;
}