#ifndef GLOBALS_H
#define GLOBALS_H
#include <QElapsedTimer>
#include <QtGlobal>
#include <QVector>
#include <iostream>
#include <vector>

typedef QVector<QVector<double>> Matrix;

extern double g_nmax;
extern const double G_V_AVERAGE;
extern int g_grid_rezolution;
extern int g_grid_cell_size;
extern qint64 g_elapsed;
extern Matrix g_possibility_matrix;
extern const Matrix G_TWO_HUNDRED_MATRIX;
extern const Matrix G_HUNDRED_MATRIX;
extern const Matrix G_FIFTY_MATRIX;
#endif
