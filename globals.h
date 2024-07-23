#ifndef GLOBALS_H
#define GLOBALS_H
#include <QElapsedTimer>
#include <QtGlobal>
#include <QVector>
#include <iostream>
#include <vector>

typedef QVector<QVector<double>> Matrix;

extern double nmax;
extern double v_average;
extern int REZOLUTION;
extern int resolution_of_the_grid;
extern qint64 elapsed;
extern Matrix possibilityMatrix;
extern Matrix twoHundredMatrix;
extern Matrix hundredMatrix;
extern Matrix fiftyMatrix;
#endif
