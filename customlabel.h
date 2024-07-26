#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <QElapsedTimer>
#include <QLabel>
#include <QPainter>
#include <QVector>
#include <QDebug>
#include "globals.h"

using namespace std;

typedef QVector<QVector<double>> Matrix;

struct Segment {
    int x_coordinates;
    int y_coordinates;
    double possibility;

    Segment() : x_coordinates(0), y_coordinates(0), possibility(0.0) {}

    Segment(int _x_coordinates, int _y_coordinates, double _possibility)
        : x_coordinates(_x_coordinates), y_coordinates(_y_coordinates), possibility(_possibility) {}
};

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    int ipp_coordinates = g_grid_rezolution / 2;
    bool algoritam_continue = true;
    Matrix initialize_matrix = g_possibility_matrix;

    explicit CustomLabel(QWidget *parent = nullptr);


    void setImage(const QString &file_path, int width, int height);
    bool segmentExist(const QVector<Segment> &e_in_out, int x_coordinates, int y_coordinates);
    void minPossibility();
    void algorithmPMA();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap pixmap;
    QVector<Segment> e_in;
    QVector<Segment> e_out;
};

#endif
