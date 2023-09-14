#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>
#include <vector>

class videothread : public QThread
{
    Q_OBJECT

private:
    int m_device;
    cv::VideoCapture VideoCap;
    cv::Mat *m_frame;

    bool *FaceDetect;
    bool *EyesDetect;
    bool *LipsDetect;

    std::vector<cv::Rect> m_eyes;
    std::vector<cv::Rect> m_lips;
    std::vector<cv::Rect> m_face;

public:
    videothread(QObject *parent = 0) = delete;
    explicit videothread(int device, cv::Mat *frame, bool *face, bool *eyes, bool *lips,
                         QObject *parent = 0, bool b = false);
    virtual ~videothread ();
    virtual void run();
    bool Stop;

signals:
    void update();

public slots:

};
#endif // VIDEOTHREAD_H
