#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#define __CL_ENABLE_EXCEPTIONS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
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
#include </Program Files (x86)/OCL_SDK_Light/include/CL/cl.hpp>
#include <windows.h>

using namespace std;

typedef QVector<QVector<double>> Matrix;

struct Segment {
    int x_coordinates;
    int y_coordinates;
    double n;

    Segment() : x_coordinates(0), y_coordinates(0), n(0.0) {}

    Segment(int _x_coordinates, int _y_coordinates, double _n)
        : x_coordinates(_x_coordinates), y_coordinates(_y_coordinates), n(_n) {}
};

struct NeighborData {
    int x_coordinates;
    int y_coordinates;
    double n;
    int e_out_index;

    NeighborData() : x_coordinates(0), y_coordinates(0), n(0.0), e_out_index(-1) {}

    NeighborData(int _x_coordinates, int _y_coordinates, double _n, int _e_out_index)
        : x_coordinates(_x_coordinates), y_coordinates(_y_coordinates), n(_n), e_out_index(_e_out_index) {}
};

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    ~CustomLabel();

    int ipp_coordinates = g_grid_rezolution / 2;
    bool algoritam_continue = true;
    Matrix initialize_matrix = g_passability_matrix;

    explicit CustomLabel(QWidget *parent = nullptr);


    void setImage(const QString &file_path, int width, int height);
    void getClosestSegmentToIPP();
    void algorithmPMA();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap pixmap;
    QVector<Segment> e_in;
    QVector<Segment> e_out;
    cl::Platform platform;
    cl::Device device;
    cl::Context context;
    cl::CommandQueue queue;
    cl::Program program;
    cl::Kernel kernel;
};

#endif
