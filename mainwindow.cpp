#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "C:/Users/ROSTO/Downloads/", tr("Image Files (*.png *.jpg *.bmp)"));
    img = cv::imread(fileName.toStdString());
    image= QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);

    connect(ui->Origin, SIGNAL(released()), this, SLOT(OriginPressed()));
    connect(ui->Blur, SIGNAL(released()), this, SLOT(BlurPressed()));
    connect(ui->GaussianBlur, SIGNAL(released()), this, SLOT(GaussianBlurPressed()));
    connect(ui->Threshold, SIGNAL(released()), this, SLOT(ThresholdPressed()));
    connect(ui->Gray, SIGNAL(released()), this, SLOT(GrayPressed()));
    connect(ui->HSV, SIGNAL(released()), this, SLOT(HSVPressed()));
    connect(ui->Canny, SIGNAL(released()), this, SLOT(CannyPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OriginPressed(){
    image= QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
}

void MainWindow::BlurPressed(){
    cv::Mat img_blur10x10;
    cv::blur(img, img_blur10x10, cv::Size(10,10));
    image= QImage(img_blur10x10.data, img_blur10x10.cols, img_blur10x10.rows, img_blur10x10.step, QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
}

void MainWindow::GaussianBlurPressed(){
    cv::Mat img_Gblur5x5;
    cv::GaussianBlur(img, img_Gblur5x5, cv::Size(5,5), 0);
    image= QImage(img_Gblur5x5.data, img_Gblur5x5.cols, img_Gblur5x5.rows, img_Gblur5x5.step, QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
}

void MainWindow::ThresholdPressed(){
    cv::Mat img_threshold;
    cv::threshold(img, img_threshold, 125, 255, 3);
    image= QImage(img_threshold.data, img_threshold.cols, img_threshold.rows, img_threshold.step, QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
}

void MainWindow::GrayPressed(){
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_RGB2GRAY, 0);
    image= QImage(img_gray.data, img_gray.cols, img_gray.rows, img_gray.step, QImage::Format_Grayscale8).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
}

void MainWindow::HSVPressed(){
    cv::Mat img_hsv;
    cv::cvtColor(img, img_hsv, cv::COLOR_RGB2HSV, 0);
    image= QImage(img_hsv.data, img_hsv.cols, img_hsv.rows, img_hsv.step, QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
}

void MainWindow::CannyPressed(){
    cv::Mat img_canny;
    cv::Canny(img, img_canny, 0, 100, 3);
    image= QImage(img_canny.data, img_canny.cols, img_canny.rows, img_canny.step, QImage::Format_Grayscale8).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
}
