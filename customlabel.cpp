// customlabel.cpp
#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent)
{
}

void CustomLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    double cellSize = qMin(width(), height()) / REZOLUTION;

    painter.setPen(Qt::transparent);
    for (int x = 0; x < REZOLUTION; ++x)
    {
        for (int y = 0; y < REZOLUTION; ++y)
        {
            painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
        }
    }

    painter.setBrush(Qt::red);
    for (const auto &segment : Ein)
    {
        painter.drawRect(segment.y * cellSize, segment.x * cellSize, cellSize, cellSize);
    }

    QPoint center(width() / 2, height() / 2);
    double radius = (400.0 / 10000.0) * nmax * resolution_of_the_grid;
    painter.setPen(Qt::red);
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(center, radius, radius);
}

void CustomLabel::setImage(const QString &filePath, int width, int height)
{
    QElapsedTimer timer;
    timer.start();
    pixmap.load(filePath);
    setPixmap(pixmap.scaled(width, height));
    Ein.push_back(Segment(IPP_COORDINATES, IPP_COORDINATES, 0));
    algorithmPMA();
    update();
    elapsed = timer.elapsed();
}

bool CustomLabel::segmentExist(const QVector<Segment> &EInOut, int x, int y)
{
    for (const auto &item : EInOut)
    {
        if (item.x == x && item.y == y)
            return true;
    }
    return false;
}

void CustomLabel::minPossibility()
{
    auto minPossibilitySegment = min_element(Eout.begin(), Eout.end(),[](const Segment &a, const Segment &b){ return a.possibility < b.possibility;});
    int minIndex = distance(Eout.begin(), minPossibilitySegment);

    if (minPossibilitySegment->possibility > nmax)
    {
        ALGORITAM_CONTINUE = false;
    }
    else
    {
        Ein.push_back(Segment(minPossibilitySegment->x, minPossibilitySegment->y, minPossibilitySegment->possibility));
        Eout.erase(Eout.begin() + minIndex);
    }
}

void CustomLabel::algorithmPMA()
{
    for (size_t segment = 0; segment < Ein.size(); ++segment)
    {
        auto &itemIn = Ein[segment];

        int rows = possibilityMatrix.size();
        int cols = possibilityMatrix[0].size();
        for (int i = max(0, itemIn.x - 1); i <= min(rows - 1, itemIn.x + 1); ++i)
        {
            for (int j = max(0, itemIn.y - 1); j <= min(cols - 1, itemIn.y + 1); ++j)
            {
                if (i == itemIn.x && j == itemIn.y)
                    continue;

                if (segmentExist(Ein, i, j))
                    continue;

                double n;
                if (i == itemIn.x || j == itemIn.y)
                {
                    n = itemIn.possibility + possibilityMatrix[i][j];
                }
                else
                {
                    n = itemIn.possibility + sqrt(2) * possibilityMatrix[i][j];
                }

                if (segmentExist(Eout, i, j))
                {
                    for (auto &item : Eout)
                    {
                        if (item.x == i && item.y == j)
                        {
                            if (n < item.possibility)
                                item.possibility = n;
                            break;
                        }
                    }
                }
                else
                {
                    Eout.push_back(Segment(i, j, n));
                }
            }
        }
        minPossibility();

        if (!ALGORITAM_CONTINUE; REZOLUTION * REZOLUTION == Ein.size())
            break;
    }
}
