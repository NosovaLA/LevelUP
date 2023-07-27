#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

/*переменные для математики*/
double result = 0.0;
bool triggerplus = false;
bool triggerminus = false;
bool triggerdivision = false;
bool triggermultiply = false;
bool triggersqrt = false;
bool triggerprocent = false;
QString savestringdata = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(result));
    connect(ui->Button_0,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_1,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_2,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_3,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_4,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_5,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_6,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_7,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_8,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_9,SIGNAL(released()),this,SLOT(PushButtonNumber()));
    connect(ui->Button_dot,SIGNAL(released()),this,SLOT(PushButtonDot()));
    connect(ui->Button_equals,SIGNAL(released()),this,SLOT(PushButtonEquals()));
    connect(ui->Button_clear,SIGNAL(released()),this,SLOT(PushButtonClear()));
    connect(ui->Button_plusminus,SIGNAL(released()),this,SLOT(PushButtonPlusMinus()));
    connect(ui->Button_plus,SIGNAL(released()),this,SLOT(PushButtonMath()));
    connect(ui->Button_minus,SIGNAL(released()),this,SLOT(PushButtonMath()));
    connect(ui->Button_multiply,SIGNAL(released()),this,SLOT(PushButtonMath()));
    connect(ui->Button_division,SIGNAL(released()),this,SLOT(PushButtonMath()));
    connect(ui->Button_procent,SIGNAL(released()),this,SLOT(PushButtonMath()));
    connect(ui->Button_sqrt,SIGNAL(released()),this,SLOT(PushButtonMath()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::PushButtonNumber() // ф-ия отображения цифр
{
    QPushButton *Button = (QPushButton*)sender();
    QString NameButton = Button->text();
    QString DisplayValue = ui->Display->text();

    savestringdata += NameButton;

    ui->Display->setText(savestringdata);
}

void MainWindow::PushButtonClear() // ф-ия очистки экрана
{
    ui->Display->setText("0");
    result = 0.0;
}

void MainWindow::PushButtonMath() // ф-ия нажатия кнопок с мат.операциями
{
    QPushButton *Button = (QPushButton*)sender();
    QString NameButton = Button->text();

    triggerplus = false;
    triggerminus = false;
    triggerdivision = false;
    triggermultiply = false;
    triggersqrt = false;
    triggerprocent = false;

    if (NameButton == "+") triggerplus = true;
    else if (NameButton == "-") triggerminus = true;
    else if (NameButton == "×") triggermultiply = true;
    else if (NameButton == "÷") triggerdivision = true;
    else if (NameButton == "%") triggerprocent = true;
    else if (NameButton == "√") triggersqrt = true;

    QString DisplayValue = ui->Display->text();
    result = DisplayValue.toDouble();
    savestringdata = "";
}

void MainWindow::PushButtonEquals() // ф-ия обработки кнопки равно
{
    QString DisplayValue = ui->Display->text();
    double SecendNumber = DisplayValue.toDouble();

    if (triggerplus) result += SecendNumber;
    else if (triggerminus) result -= SecendNumber;
    else if (triggermultiply) result *= SecendNumber;
    else if (triggerdivision) result /= SecendNumber;
    else if (triggersqrt) result = sqrt(result);
    else if (triggerprocent) result = result * SecendNumber / 100;

    ui->Display->setText(QString::number(result));
}


void MainWindow::PushButtonDot() // ф-ия обработки точки
{
     QString DisplayValue = ui->Display->text();
     if (DisplayValue.indexOf(".") == -1)
        ui->Display->setText(DisplayValue + ".");
}

void MainWindow::PushButtonPlusMinus() // ф-ия обработки плюс-минус
{
    QString DisplayValue = ui->Display->text();

    double PMValue = DisplayValue.toDouble();
    PMValue *= -1;

    ui->Display->setText(QString::number(PMValue));
}

























