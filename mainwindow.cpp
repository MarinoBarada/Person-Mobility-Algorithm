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
    double v = G_V_AVERAGE  * person_category_factor * faktor;
    double s_max = v * time_tnterval;
    g_nmax= s_max / g_grid_cell_size;

    ImageDialog image_dialog;
    image_dialog.setModal(true);
    image_dialog.exec();
}


void MainWindow::on_category_dropdown_currentTextChanged(const QString &option)
{
    person_category_factor =  ui->category_dropdown->currentData().toDouble();
}


void MainWindow::on_timeEdit_userTimeChanged(const QTime &time)
{
    time_tnterval  = time.hour() * 3600 + time.minute() * 60 + time.second();
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
            g_grid_cell_size=50;
            g_passability_matrix=G_TWO_HUNDRED_MATRIX;
            g_grid_rezolution = 10000/g_grid_cell_size;
            break;
        case 100:
            g_grid_cell_size=100;
            g_passability_matrix=G_HUNDRED_MATRIX;
            g_grid_rezolution = 10000/g_grid_cell_size;
            break;
        case 200:
            g_grid_cell_size=200;
            g_passability_matrix=G_FIFTY_MATRIX;
            g_grid_rezolution = 10000/g_grid_cell_size;
            break;
        default:
            g_grid_cell_size=200;
            g_passability_matrix=G_FIFTY_MATRIX;
            g_grid_rezolution = 10000/g_grid_cell_size;
            break;
        }
}

