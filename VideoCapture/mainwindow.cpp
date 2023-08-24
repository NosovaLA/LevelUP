#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Video = cv::VideoCapture(0);
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer->start(25);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update(){
    Video.read(img);
    image= QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->label->setPixmap(pixel);
    if (ui->Origin->isChecked()){
        image= QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    } else if (ui->Blur->isChecked()){
        cv::Mat img_blur10x10;
        cv::blur(img, img_blur10x10, cv::Size(10,10));
        image= QImage(img_blur10x10.data, img_blur10x10.cols, img_blur10x10.rows, img_blur10x10.step, QImage::Format_RGB888).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    } else if (ui->GaussianBlur->isChecked()){
        cv::Mat img_Gblur5x5;
        cv::GaussianBlur(img, img_Gblur5x5, cv::Size(5,5), 0);
        image= QImage(img_Gblur5x5.data, img_Gblur5x5.cols, img_Gblur5x5.rows, img_Gblur5x5.step, QImage::Format_RGB888).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    } else if (ui->Threshold->isChecked()){
        cv::Mat img_threshold;
        cv::threshold(img, img_threshold, 125, 255, 3);
        image= QImage(img_threshold.data, img_threshold.cols, img_threshold.rows, img_threshold.step, QImage::Format_RGB888).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    } else if (ui->Gray->isChecked()){
        cv::Mat img_gray;
        cv::cvtColor(img, img_gray, cv::COLOR_RGB2GRAY, 0);
        image= QImage(img_gray.data, img_gray.cols, img_gray.rows, img_gray.step, QImage::Format_Grayscale8).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    } else if (ui->HSV->isChecked()){
        cv::Mat img_hsv;
        cv::cvtColor(img, img_hsv, cv::COLOR_RGB2HSV, 0);
        image= QImage(img_hsv.data, img_hsv.cols, img_hsv.rows, img_hsv.step, QImage::Format_RGB888).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    } else if (ui->Canny->isChecked()){
        cv::Mat img_canny;
        cv::Canny(img, img_canny, 0, 100, 3);
        image= QImage(img_canny.data, img_canny.cols, img_canny.rows, img_canny.step, QImage::Format_Grayscale8).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    } else if (ui->Draw->isChecked()){
        cv::Mat img_clone;
        img_clone = img.clone();
        cv::line(img_clone, {10,10}, {90,10}, {255,255,255}, 3);
        cv::circle(img_clone, {50,70}, 40, {255,255,255}, 3);
        cv::rectangle(img_clone, {10,130}, {90,210}, {255,255,255}, 3);

        std::vector<cv::Point> tmp = {{10,230}, {10,300}, {60,300}, {60,230}};
        const cv::Point* elementPoints[1] = { &tmp[0] };
        int numberOfPoints = (int)tmp.size();
        cv::fillPoly (img_clone, elementPoints, &numberOfPoints, 1, cv::Scalar (255, 255, 255), 8);

        cv::putText(img_clone, "Line", {95,25}, cv::FONT_HERSHEY_PLAIN, 2, {255,255,255});
        cv::putText(img_clone, "Circle", {95,80}, cv::FONT_HERSHEY_PLAIN, 2, {255,255,255});
        cv::putText(img_clone, "Rectangle", {95,180}, cv::FONT_HERSHEY_PLAIN, 2, {255,255,255});
        cv::putText(img_clone, "FillPoly", {95,270}, cv::FONT_HERSHEY_PLAIN, 2, {255,255,255});

        image= QImage(img_clone.data, img_clone.cols, img_clone.rows, img_clone.step, QImage::Format_RGB888).rgbSwapped();
        pixel = QPixmap::fromImage(image);
        ui->label->setPixmap(pixel);
    }
}

