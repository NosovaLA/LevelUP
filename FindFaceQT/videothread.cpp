#include "videothread.h"

#include <QDebug>
#include <QMutex>

videothread::videothread(int device, cv::Mat *frame, bool *face, bool *eyes,
                         bool *lips, QObject *parent, bool b) :
    QThread(parent), Stop(b), m_device(device), VideoCap(0), m_frame(frame)
{
    FaceDetect = face;
    EyesDetect = eyes;
    LipsDetect = lips;
}

void videothread::run(){
    QMutex mtx;
    cv::Mat mat_capture;
    cv::Mat mat_process;
    cv::CascadeClassifier casc_eyes;
    cv::CascadeClassifier casc_lips;
    cv::CascadeClassifier casc_face;

    casc_eyes.load("../CV_Face/haarcascade_eye_tree_eyeglasses.xml");
    casc_lips.load("../CV_Face/haarcascade_smile.xml");
    casc_face.load("../CV_Face/haarcascade_frontalface_alt.xml");

    if (!VideoCap.isOpened()){
        mtx.lock();
        Stop = true;
        mtx.unlock();
    }

    while(Stop == false){
        VideoCap.read(mat_capture);
        cv::cvtColor(mat_capture, mat_process, cv::COLOR_BGR2GRAY);

        mtx.lock();
        if (*FaceDetect){
            casc_face.detectMultiScale(mat_process, m_face);
            if (m_face.size () != 0) {
                for (auto it : m_face) {
                    cv::rectangle (mat_capture, it, cv::Scalar{ 0, 255, 0 },
                                   3, cv::LINE_8);
                }
            }
        }
        if (*EyesDetect){
            casc_eyes.detectMultiScale(mat_process, m_eyes);
            if (m_eyes.size () != 0) {
                for (auto it : m_eyes) {
                    cv::rectangle (mat_capture, it, cv::Scalar{ 0, 255, 255 },
                                   3, cv::LINE_8);
                }
            }
        }
        if (*LipsDetect){
            casc_lips.detectMultiScale(mat_process, m_lips);
            if (m_lips.size () != 0) {
                for (auto it : m_lips) {
                    cv::rectangle (mat_capture, it, cv::Scalar{ 255, 255, 0 },
                                   3, cv::LINE_8);
                }
            }
        }

        *m_frame = mat_capture.clone();
        mtx.unlock();
        emit update();
    }
}

videothread::~videothread () {
    VideoCap.release();
}
