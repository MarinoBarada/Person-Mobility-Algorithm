#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H
#include <QDialog>
#include "customlabel.h"
#include <chrono>

namespace Ui {
class ImageDialog;
}

class ImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageDialog(QWidget *parent = nullptr);
    ~ImageDialog();
    QString formatElapsedTime(qint64 milliseconds);

private:
    Ui::ImageDialog *ui;
    CustomLabel *customLabel;
};

#endif
