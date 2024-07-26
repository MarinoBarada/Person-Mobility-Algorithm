#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent)
{
}

void CustomLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    double cell_size = qMin(width(), height()) / g_grid_rezolution;

    painter.setPen(Qt::transparent);
    for (int x = 0; x < g_grid_rezolution; ++x) {
        for (int y = 0; y < g_grid_rezolution; ++y) {
            painter.drawRect(x * cell_size, y * cell_size, cell_size, cell_size);
        }
    }

    painter.setBrush(Qt::red);
    for (const auto &segment : e_in) {
        painter.drawRect(segment.y_coordinates * cell_size, segment.x_coordinates * cell_size, cell_size, cell_size);
    }

    QPoint center(width() / 2, height() / 2);
    double radius = (400.0 / 10000.0) * g_nmax * g_grid_cell_size;
    painter.setPen(Qt::red);
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(center, radius, radius);
}

void CustomLabel::setImage(const QString &file_path, int width, int height)
{
    QElapsedTimer timer;
    timer.start();
    pixmap.load(file_path);
    setPixmap(pixmap.scaled(width, height));
    e_in.push_back(Segment(ipp_coordinates, ipp_coordinates, 0));
    algorithmPMA();
    update();
    g_elapsed = timer.elapsed();

    int cell_surface = g_grid_cell_size * g_grid_cell_size;
    int assumed_surface_area = cell_surface * e_in.length();

    if (assumed_surface_area >= 10000.0) {
        double assumed_area_in_hectares = assumed_surface_area / 10000.0;
        qDebug() << "Pretpostavljena povrsina pomocu algoritma:" << QString::number(assumed_area_in_hectares, 'f', 2) << "ha";
    } else {
        qDebug() << "Pretpostavljena povrsina pomocu algoritma:" << QString::number(assumed_surface_area, 'f', 2) << "m2";
    }

    double radius = g_nmax * g_grid_cell_size;
    double circle_area = M_PI * pow(radius, 2);

    if (circle_area >= 10000.0) {
        double circle_area_in_hectares = circle_area / 10000.0;
        qDebug() << "Povrsina kruga s obicnom metodom prstena:" << QString::number(circle_area_in_hectares, 'f', 2) << "ha";
    } else {
        qDebug() << "Povrsina kruga s obicnom metodom prstena:" << QString::number(circle_area, 'f', 2) << "m2";
    }
    qDebug() << "---------------------------------------------------------------------------\n";
}

bool CustomLabel::segmentExist(const QVector<Segment> &e_in_out, int x_coordinates, int y_coordinates)
{
    for (const auto &segment : e_in_out) {
        if (segment.x_coordinates == x_coordinates && segment.y_coordinates == y_coordinates)
            return true;
    }
    return false;
}

void CustomLabel::minPossibility()
{
    auto minPossibilitySegment = min_element(e_out.begin(), e_out.end(),[](const Segment &a, const Segment &b){ return a.possibility < b.possibility;});
    int minIndex = distance(e_out.begin(), minPossibilitySegment);

    if (minPossibilitySegment->possibility > g_nmax) {
        algoritam_continue = false;
    }
    else {
        e_in.push_back(Segment(minPossibilitySegment->x_coordinates, minPossibilitySegment->y_coordinates, minPossibilitySegment->possibility));
        e_out.erase(e_out.begin() + minIndex);
    }
}

void CustomLabel::algorithmPMA()
{
    for (size_t segment_index = 0; segment_index < e_in.size(); ++segment_index) {
        auto &segment_in = e_in[segment_index];

        int rows = g_possibility_matrix.size();
        int cols = g_possibility_matrix[0].size();
        for (int i = max(0, segment_in.x_coordinates - 1); i <= min(rows - 1, segment_in.x_coordinates + 1); ++i) {
            for (int j = max(0, segment_in.y_coordinates - 1); j <= min(cols - 1, segment_in.y_coordinates + 1); ++j) {
                if (i == segment_in.x_coordinates && j == segment_in.y_coordinates)
                    continue;

                if (segmentExist(e_in, i, j))
                    continue;

                double n = (i == segment_in.x_coordinates || j == segment_in.y_coordinates)
                               ? segment_in.possibility + g_possibility_matrix[i][j]
                               : segment_in.possibility + sqrt(2) * g_possibility_matrix[i][j];

                bool in_e_out = false;
                for (auto &segment : e_out) {
                    if (segment.x_coordinates == i && segment.y_coordinates == j) {
                        in_e_out = true;
                        segment.possibility = std::min(segment.possibility, n);
                        break;
                    }
                }

                if (!in_e_out) {
                    e_out.emplace_back(i, j, n);
                }
            }
        }
        minPossibility();

        if (!algoritam_continue; g_grid_rezolution * g_grid_rezolution == e_in.size())
            break;
    }
}
