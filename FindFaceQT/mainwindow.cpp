#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMutex>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    vThread(0)
{
    ui->setupUi(this);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer->start(25);

    FaceDetect = ui->checkFaceDetect->isChecked();
    EyesDetect = ui->checkEyesDetect->isChecked();
    LipsDetect = ui->checkLipsDetect->isChecked();
    connect(ui->StartCap, &QPushButton::clicked, this,
            &MainWindow::StartCapture);
    connect(ui->checkFaceDetect, &QCheckBox::clicked, this,
            &MainWindow::CheckBoxChecked);
    connect(ui->checkEyesDetect, &QCheckBox::clicked, this,
            &MainWindow::CheckBoxChecked);
    connect(ui->checkLipsDetect, &QCheckBox::clicked, this,
            &MainWindow::CheckBoxChecked);
}

void MainWindow::Update(){
    image= QImage(img.data, img.cols, img.rows, img.step,
                  QImage::Format_RGB888).rgbSwapped();
    pixel = QPixmap::fromImage(image);
    ui->VideoCap->setPixmap(pixel);
}

void MainWindow::CheckBoxChecked(){
    FaceDetect = ui->checkFaceDetect->isChecked();
    EyesDetect = ui->checkEyesDetect->isChecked();
    LipsDetect = ui->checkLipsDetect->isChecked();
}


void MainWindow::StartCapture(){
    QMutex mtx;
    if (vThread != nullptr){
        mtx.lock();
        vThread->Stop = true;
        mtx.unlock();
        vThread->wait();
        disconnect(vThread, &videothread::update, this, &MainWindow::Update);
        delete vThread;
        vThread = 0;
        ui->statusbar->showMessage("stop");
    } else {
        vThread = new videothread(0, &img, &FaceDetect, &EyesDetect, &LipsDetect);
        vThread->Stop = false;
        connect(vThread, &videothread::update, this, &MainWindow::Update);
        vThread->start();
        ui->statusbar->showMessage("running");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

