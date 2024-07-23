#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QLabel>
#include <QPainter>
#include <QVector>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <QElapsedTimer>
#include <QPainter>
#include "globals.h"

using namespace std;

typedef QVector<QVector<double>> Matrix;

struct Segment {
    int x;
    int y;
    double possibility;

    Segment(int _x, int _y, double _possibility)
        : x(_x), y(_y), possibility(_possibility) {}
};

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    int IPP_COORDINATES = REZOLUTION / 2;
    bool ALGORITAM_CONTINUE = true;
    Matrix initialize_matrix = possibilityMatrix;

    explicit CustomLabel(QWidget *parent = nullptr);


    void setImage(const QString &filePath, int width, int height);
    bool segmentExist(const QVector<Segment> &EInOut, int x, int y);
    void minPossibility();
    void algorithmPMA();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap pixmap;
    QVector<Segment> Ein;
    QVector<Segment> Eout;
};

#endif
