#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>

#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QTimer>

#include "videothread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    videothread *vThread;

private:
    Ui::MainWindow *ui;
    cv::Mat img;
    QImage image;
    QPixmap pixel;
    cv::VideoCapture Video;

    bool FaceDetect;
    bool EyesDetect;
    bool LipsDetect;

private slots:
    void StartCapture();
    void CheckBoxChecked();
    void Update();
};
#endif // MAINWINDOW_H
