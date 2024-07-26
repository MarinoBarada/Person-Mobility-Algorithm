#include "imagedialog.h"
#include "ui_imagedialog.h"

ImageDialog::ImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageDialog)
{
    ui->setupUi(this);

    CustomLabel *custom_label = new CustomLabel(ui->image);

    custom_label->setImage("C:/Users/MarinoBarada/Documents/PMA_app/image.png" , ui->image->width(), ui->image->height());

    ui->timeLabel->setText("Vrijeme izvedbe:: " + formatElapsedTime(g_elapsed));
}

ImageDialog::~ImageDialog()
{
    delete ui;
}

QString ImageDialog::formatElapsedTime(qint64 milliseconds) {
    if (milliseconds < 1000) {
        return QString::number(milliseconds) + " millisekunda";
    } else if (milliseconds < 60000) {
        double seconds = milliseconds / 1000.0;
        return QString::number(seconds, 'f', 2) + " sekunda";
    } else {
        double minutes = milliseconds / 60000.0;
        return QString::number(minutes, 'f', 2) + " minuta";
    }
}
