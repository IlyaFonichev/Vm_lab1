#ifndef FONICHEV_LAB1_SPLINES_H
#define FONICHEV_LAB1_SPLINES_H

void spline(int n, const double *x, const double *y, double *b, double *c, double *d);

double seval(int n, const double *u,
             const double *x, double *y, const double *b, const double *c,
             const double *d);

#endif //FONICHEV_LAB1_SPLINES_H