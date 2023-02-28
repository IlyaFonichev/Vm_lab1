#include <iostream>
#include "quanc8.h"
#include "StreamTable.h"
#include <cmath>
#include "lagrange.h"
#include "SPLINES.H"


using namespace std;


double fun(double t) {
    if (t == 0.0) return 0.0;
    else return (1 - cos(t)) / t;
}

double correctValue(double x) {
    double result;
    double errest;
    int nofun;
    double flag;
    quanc8(fun, 0.0, x, 0.0, 0.0, &result, &errest, &nofun, &flag);
    return result;
}

int main() {

    // вычисление всех значений фнкции для заданных значений аргумента
    double count = 2.0;
    double values[11];
    double keys[11];
    correctValue(0.0);
    for (int i = 0; i < 11; i++) {
        keys[i] = count;
        values[i] = correctValue(count);
        count += 0.1;
    }

    // построение интерполяционного полинома Лагранжа 10-й степени
    double count1 = 2.05;
    double lagrangeValues[10];
    for (double &lagrangeValue : lagrangeValues) {
        lagrangeValue = lagrange(10, keys, values, 2.0);
        count1 += 0.1;
    }

    // построение сплайна и вычисление коэффициентов
    count1 = 2.05;
    double b[11];
    double c[11];
    double d[11];
    double splineValues[10];
    spline(10, keys, values, b, c, d);
    for (double &splineValue : splineValues) {
        splineValue = seval(10, &count1, keys, values, b, c, d);
        count1 += 0.1;
    }

    //вычисление значений с помощью QUANC8
    double quancValues[10];
    count1 = 2.05;
    for (double &quancValue : quancValues) {
        quancValue = correctValue(count1);
        count1 += 0.1;
    }

    // сравнение и вывод результатов в табличку
    StreamTable st(std::cout);
    st.AddCol(9);
    st.AddCol(9);
    st.AddCol(9);

    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');
    st.SetDelimCol(true, '|');

    cout << "Values" << endl;
    st << "QUANC8" << "Spline" << "Lagrange";
    enum {nr = 10};
    for (int i = 0; i < nr; i++) {
        st << quancValues[i] << splineValues[i] << lagrangeValues[i];
    }

    cout << "Comparison" << endl;
    st << "QUANC8" << "Spline Diff" << "Lagrange Diff";
    for (int i = 0; i < nr; i++) {
        st << quancValues[i] << quancValues[i] - splineValues[i] << quancValues[i] - lagrangeValues[i];
    }
}


