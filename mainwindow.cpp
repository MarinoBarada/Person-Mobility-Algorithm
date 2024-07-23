#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(100);
    ui->horizontalSlider->setValue(100);

    ui->category_dropdown->addItem("Dijete (0-12 godina)", 0.5);
    ui->category_dropdown->addItem("Tinejdžer (13-19 godina)", 0.7);
    ui->category_dropdown->addItem("Odrasla osoba (20-59 godina)", 1.0);
    ui->category_dropdown->addItem("Starija osoba (60+ godina)", 0.8);
    ui->category_dropdown->addItem("Iskusni Planinar", 1.2);
    ui->category_dropdown->addItem("Osoba s Medicinskim Stanjima", 0.6);

    ui->segment_size_dropdown->addItem("200m x 200m", 200);
    ui->segment_size_dropdown->addItem("100m x 100m", 100);
    ui->segment_size_dropdown->addItem("50m x 50m", 50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    double v = v_average * personCategoryFactor * faktor;
    double sMax = v * timeInterval;
    nmax= sMax / resolution_of_the_grid;

    ImageDialog imageDialog;
    imageDialog.setModal(true);
    imageDialog.exec();
}


void MainWindow::on_category_dropdown_currentTextChanged(const QString &option)
{
    personCategoryFactor =  ui->category_dropdown->currentData().toDouble();
}


void MainWindow::on_timeEdit_userTimeChanged(const QTime &time)
{
    timeInterval  = time.hour() * 3600 + time.minute() * 60 + time.second();
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    faktor = static_cast<double>(value) / 100;
    ui->labelForDisplay_3->setText("Odabrani faktor: " + QString::number(faktor));
}


void MainWindow::on_segment_size_dropdown_currentTextChanged(const QString &option)
{
    switch (ui->segment_size_dropdown->currentData().toInt()) {
        case 50:
            resolution_of_the_grid=50;
            possibilityMatrix=twoHundredMatrix;
            REZOLUTION = 10000/resolution_of_the_grid;
            break;
        case 100:
            resolution_of_the_grid=100;
            possibilityMatrix=hundredMatrix;
            REZOLUTION = 10000/resolution_of_the_grid;
            break;
        case 200:
            resolution_of_the_grid=200;
            possibilityMatrix=fiftyMatrix;
            REZOLUTION = 10000/resolution_of_the_grid;
            break;
        default:
            resolution_of_the_grid=200;
            possibilityMatrix=fiftyMatrix;
            REZOLUTION = 10000/resolution_of_the_grid;
            break;
        }
}

