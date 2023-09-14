#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QPair>
#include <vector>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef QPair<float, float> FloatPair;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    cv::Mat orig_img, cpyorig_img, HSV_img, path_img;
    cv::Mat mat_trapeze, mat_transform_trapeze, mat_HSV_trapeze, mat_roi;
    cv::VideoCapture orig_video;
    cv::Point2f trapeze[4], transform_trapeze[4];
    cv::Rect rect;
    cv::Point path[4], transform_path[4];
    QImage image, debug_image;
    QPixmap pixel, debug_pixel;
    std::vector<cv::Point> left_points, right_points;
    FloatPair koeff_left, koeff_right;

    FloatPair Least_squares (std::vector<cv::Point> &point_v);

private slots:
    void OpenVideo();
    void UpdateData();
};
#endif // MAINWINDOW_H
