#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <stdio.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    orig_video()
{
    ui->setupUi(this);

    QTimer* timer = new QTimer(this);
    timer->start(25);

    ui->TrapezeTopSize->setMinimum(2);
    ui->TrapezeTopSize->setMaximum(800);
    ui->TrapezeBottomSize->setMinimum(2);
    ui->TrapezeBottomSize->setMaximum(1200);
    ui->TrapezeHeight->setMinimum(2);
    ui->TrapezeHeight->setMaximum(600);

    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateData()));
    connect(ui->OpenFile, &QPushButton::clicked, this, &MainWindow::OpenVideo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenVideo(){

    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Video"),
                                "./", tr("Video Files (*.mp4 *.avi *wmv)"));
    orig_video = cv::VideoCapture(file_name.toStdString());
}

void MainWindow::UpdateData(){
    if (orig_video.read(orig_img)){
        orig_img.copyTo(cpyorig_img);

        if (ui->VideoData->isChecked()){
            int frame_current = orig_video.get(cv::CAP_PROP_POS_FRAMES);
            std::string s = std::to_string(frame_current);
            cv::putText(orig_img, s, {10,50},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
            cv::putText(orig_img, "CURRENT FRAME", {220,50},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
            cv::putText(cpyorig_img, s, {10,50},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
            cv::putText(cpyorig_img, "CURRENT FRAME", {220,50},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
            int end_frame = orig_video.get(cv::CAP_PROP_FRAME_COUNT);
            s = std::to_string(end_frame);
            cv::putText(orig_img, s, {10,100},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
            cv::putText(orig_img, "END FRAME", {220,100},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
            cv::putText(cpyorig_img, s, {10,100},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
            cv::putText(cpyorig_img, "END FRAME", {220,100},
                        cv::FONT_HERSHEY_PLAIN, 4, {255,255,255}, 3);
        }

        trapeze[0] = cv::Point2f(cpyorig_img.cols/2-(ui->TrapezeTopSize->value())/2,
                                 cpyorig_img.rows-(ui->TrapezeHeight->value()));
        trapeze[1] = cv::Point2f(cpyorig_img.cols/2+(ui->TrapezeTopSize->value())/2,
                                 cpyorig_img.rows-(ui->TrapezeHeight->value()))  ;
        trapeze[2] = cv::Point2f(cpyorig_img.cols/2+(ui->TrapezeBottomSize->value())/2,
                                 cpyorig_img.rows-50);
        trapeze[3] = cv::Point2f(cpyorig_img.cols/2-(ui->TrapezeBottomSize->value())/2,
                                 cpyorig_img.rows-50);

        transform_trapeze[0] = cv::Point2f(0, 0);
        transform_trapeze[1] = cv::Point2f(480, 0);
        transform_trapeze[2] = cv::Point2f(480, 272);
        transform_trapeze[3] = cv::Point2f(0, 272);

        mat_transform_trapeze = cv::getPerspectiveTransform(trapeze,
                                                            transform_trapeze);
        cv::warpPerspective(cpyorig_img, mat_trapeze, mat_transform_trapeze,
                            cv::Size(480, 272));

        cv::circle(cpyorig_img, trapeze[0], 2, {0,255,0}, 3);
        cv::circle(cpyorig_img, trapeze[1], 2, {0,255,0}, 3);
        cv::circle(cpyorig_img, trapeze[2], 2, {0,255,0}, 3);
        cv::circle(cpyorig_img, trapeze[3], 2, {0,255,0}, 3);
        cv::line(cpyorig_img, trapeze[0], trapeze[1], cv::Scalar(0,255,0), 3, 8);
        cv::line(cpyorig_img, trapeze[1], trapeze[2], cv::Scalar(0,255,0), 3, 8);
        cv::line(cpyorig_img, trapeze[2], trapeze[3], cv::Scalar(0,255,0), 3, 8);
        cv::line(cpyorig_img, trapeze[3], trapeze[0], cv::Scalar(0,255,0), 3, 8);

        cv::cvtColor(orig_img, HSV_img, cv::COLOR_BGR2HSV);
        cv::inRange(HSV_img, cv::Scalar(0,0,200), cv::Scalar(70,255,255), HSV_img);

        cv::resize(HSV_img, HSV_img, cv::Size(480,272));
        debug_image = QImage(HSV_img.data, HSV_img.cols, HSV_img.rows,
                             QImage::Format_Grayscale8);
        debug_pixel = QPixmap::fromImage(debug_image);
        ui->DebugVideo->setPixmap(debug_pixel);

        if (ui->TrapezeOn->isChecked()){
            cv::resize(cpyorig_img, cpyorig_img, cv::Size(480,272));
            image = QImage(cpyorig_img.data, cpyorig_img.cols, cpyorig_img.rows,
                           QImage::Format_BGR888);
            pixel = QPixmap::fromImage(image);
            ui->OriginVideo->setPixmap(pixel);

            cv::cvtColor(mat_trapeze, mat_HSV_trapeze, cv::COLOR_BGR2HSV);
            cv::inRange(mat_HSV_trapeze, cv::Scalar(0,0,200),
                        cv::Scalar(70,255,255), mat_HSV_trapeze);

            std::vector<cv::Point> pts;
            left_points.clear();
            right_points.clear();

            for (int i=0; i < mat_HSV_trapeze.cols/40; i++){
                for (int j=0; j < mat_HSV_trapeze.rows/20; j++){
                    rect = cv::Rect(i*20, j*20, 20, 20);
                    mat_roi = cv::Mat(mat_HSV_trapeze, rect);
                    cv::findNonZero(mat_roi, pts);
                    if (pts.size() > 100){
                        left_points.emplace(left_points.end(), i*20, j*20);
                    }
                }
            }

            for (int i=mat_HSV_trapeze.cols/40; i < mat_HSV_trapeze.cols/20; i++){
                for (int j=0; j < mat_HSV_trapeze.rows/20; j++){
                    rect = cv::Rect(i*20, j*20, 20, 20);
                    mat_roi = cv::Mat(mat_HSV_trapeze, rect);
                    cv::findNonZero(mat_roi, pts);
                    if (pts.size() > 5){
                        right_points.emplace(right_points.end(), i*20, j*20);
                    }
                }
            }

            for (auto it:left_points){
                cv::rectangle(mat_trapeze, cv::Point(it.x, it.y),
                              cv::Point(it.x+20, it.y+20), cv::Scalar{0,255,0}, 1);
            }
            for (auto it:right_points){
                cv::rectangle(mat_trapeze, cv::Point(it.x, it.y),
                              cv::Point(it.x+20, it.y+20), cv::Scalar{0,0,255}, 1);
            }

            cv::Point left_line[2], right_line[2];
            koeff_left = Least_squares(left_points);
            koeff_right = Least_squares(right_points);

            left_line[0] = cv::Point(koeff_left.second*0 + koeff_left.first, 0);
            left_line[1] = cv::Point(koeff_left.second*mat_trapeze.rows + koeff_left.first, mat_trapeze.rows);

            right_line[0] = cv::Point(koeff_right.second*0 + koeff_right.first, 0);
            right_line[1] = cv::Point(koeff_right.second*mat_trapeze.rows + koeff_right.first, mat_trapeze.rows);

            cv::line(mat_trapeze, left_line[0], left_line[1],
                    cv::Scalar{0,255,0}, 3);
            cv::line(mat_trapeze, right_line[0], right_line[1],
                    cv::Scalar{0,0,255}, 3);

            cv::resize(mat_trapeze, mat_trapeze, cv::Size(480,272));
            image = QImage(mat_trapeze.data, mat_trapeze.cols, mat_trapeze.rows,
                           QImage::Format_BGR888);
            pixel = QPixmap::fromImage(image);
            ui->ProcessOrigVideo->setPixmap(pixel);

            debug_image = QImage(mat_HSV_trapeze.data, mat_HSV_trapeze.cols,
                                 mat_HSV_trapeze.rows, QImage::Format_Grayscale8);
            debug_pixel = QPixmap::fromImage(debug_image);
            ui->ProcessDebugVideo->setPixmap(debug_pixel);
        } else {
            cv::resize(orig_img, orig_img, cv::Size(480,272));
            image = QImage(orig_img.data, orig_img.cols, orig_img.rows,
                           QImage::Format_BGR888);
            pixel = QPixmap::fromImage(image);
            ui->OriginVideo->setPixmap(pixel);
        }
    }
}

FloatPair MainWindow::Least_squares (std::vector<cv::Point> &point_v){
    std::vector<cv::Point> pts;
    float x, y;
    float sx = 0, sy = 0, sxy = 0;

    if (point_v.size() == 0)
        return FloatPair(0, 0);

    for (auto it : point_v){
        x = it.y;
        sx += x;

        y = it.x;
        sy += y;

        sxy += x*y;
        pts.push_back (cv::Point(x, y));
    }

    // Average x and y
    float avg_x = sx / point_v.size ();
    float avg_y = sy / point_v.size ();

    // Average x*y
    float avg_xy = sxy / point_v.size ();

    float fx = 0;
    float fy = 0;

    for (auto it: pts)
    {
        fx += (it.x - avg_x)*(it.x - avg_x);
        fy += (it.y - avg_y)*(it.y - avg_y);
    }

    float sdev_x = std::sqrt(fx/pts.size());
    float sdev_y = std::sqrt(fy/pts.size());

    float corr = (avg_xy - avg_x*avg_y)/(sdev_x*sdev_y);

    fy = corr * sdev_y / sdev_x;
    fx = avg_y - avg_x * fy;

    return FloatPair(fx, fy);
}
