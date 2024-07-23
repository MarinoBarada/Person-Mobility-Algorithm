#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "imagedialog.h"
#include "QVariant"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_category_dropdown_currentTextChanged(const QString &arg1);
    void on_timeEdit_userTimeChanged(const QTime &time);
    void on_horizontalSlider_valueChanged(int value);
    void on_segment_size_dropdown_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    ImageDialog *imageDialog;
    double personCategoryFactor;
    int timeInterval = 0;
    double faktor = 1;

};
#endif
