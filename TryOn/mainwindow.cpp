#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QGraphicsLayout>
#include "message.h"


QString filename = "path.txt";
QString filename1 = "gamma.txt";
QString message_file = "message.txt";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //auto x = MainWindow.size();
    //ui->scrollArea->size(x));
    QPixmap pix1("../frame/frame1.png");
    QIcon ButtonIcon1(pix1);
    ui->frame1->setIcon(ButtonIcon1);
    ui->frame1->setIconSize(ui->frame1->size());

    QPixmap pix2("../frame/frame2.png");
    QIcon ButtonIcon2(pix2);
    ui->frame2->setIcon(ButtonIcon2);
    ui->frame2->setIconSize(ui->frame2->size());

    QPixmap pix3("../frame/frame3.png");
    QIcon ButtonIcon3(pix3);
    ui->frame3->setIcon(ButtonIcon3);
    ui->frame3->setIconSize(ui->frame3->size());

    QPixmap pix4("../frame/lensframe1.png");
    QIcon ButtonIcon4(pix4);
    ui->frame4->setIcon(ButtonIcon4);
    ui->frame4->setIconSize(ui->frame4->size());

    QPixmap pix5("../frame/sunglass1.png");
    QIcon ButtonIcon5(pix5);
    ui->frame5->setIcon(ButtonIcon5);
    ui->frame5->setIconSize(ui->frame5->size());

    QPixmap pix6("../frame/frame4.png");
    QIcon ButtonIcon6(pix6);
    ui->frame6->setIcon(ButtonIcon6);
    ui->frame6->setIconSize(ui->frame6->size());

    QPixmap pix7("../frame/sunglass3.png");
    QIcon ButtonIcon7(pix7);
    ui->frame7->setIcon(ButtonIcon7);
    ui->frame7->setIconSize(ui->frame7->size());

    QPixmap pix8("../frame/sunglass4.png");
    QIcon ButtonIcon8(pix8);
    ui->frame8->setIcon(ButtonIcon8);
    ui->frame8->setIconSize(ui->frame8->size());

    QPixmap pix9("../frame/sunglass5.png");
    QIcon ButtonIcon9(pix9);
    ui->frame9->setIcon(ButtonIcon9);
    ui->frame9->setIconSize(ui->frame9->size());

    QPixmap pix10("../frame/style1.png");
    QIcon ButtonIcon10(pix10);
    ui->frame10->setIcon(ButtonIcon10);
    ui->frame10->setIconSize(ui->frame10->size());

    QPixmap pix11("../frame/style2.png");
    QIcon ButtonIcon11(pix11);
    ui->frame11->setIcon(ButtonIcon11);
    ui->frame11->setIconSize(ui->frame11->size());

    QPixmap pix12("../frame/style3.png");
    QIcon ButtonIcon12(pix12);
    ui->frame12->setIcon(ButtonIcon12);
    ui->frame12->setIconSize(ui->frame12->size());

    QPixmap pix13("../frame/style4.png");
    QIcon ButtonIcon13(pix13);
    ui->frame13->setIcon(ButtonIcon13);
    ui->frame13->setIconSize(ui->frame13->size());

    QPixmap pix14("../frame/style5.png");
    QIcon ButtonIcon14(pix14);
    ui->frame14->setIcon(ButtonIcon14);
    ui->frame14->setIconSize(ui->frame14->size());

    QPixmap pix15("../frame/style6.png");
    QIcon ButtonIcon15(pix15);
    ui->frame15->setIcon(ButtonIcon15);
    ui->frame15->setIconSize(ui->frame15->size());


    QVBoxLayout * Vbox = new QVBoxLayout(this);
    QHBoxLayout * Hbox1 = new QHBoxLayout(this);
    Hbox1->addWidget(ui->frame1);
    Hbox1->addWidget(ui->frame2);
    Vbox->addItem(Hbox1);

    QHBoxLayout * Hbox2 = new QHBoxLayout(this);
    Hbox2->addWidget(ui->frame3);
    Hbox2->addWidget(ui->frame4);
    Vbox->addItem(Hbox2);

    QHBoxLayout * Hbox3 = new QHBoxLayout(this);
    Hbox3->addWidget(ui->frame5);
    Hbox3->addWidget(ui->frame6);
    Vbox->addItem(Hbox3);

    QHBoxLayout * Hbox4 = new QHBoxLayout(this);
    Hbox4->addWidget(ui->frame7);
    Hbox4->addWidget(ui->frame8);
    Vbox->addItem(Hbox4);

    QHBoxLayout * Hbox5 = new QHBoxLayout(this);
    Hbox5->addWidget(ui->frame9);
    Hbox5->addWidget(ui->frame10);
    Vbox->addItem(Hbox5);

    QHBoxLayout * Hbox6 = new QHBoxLayout(this);
    Hbox6->addWidget(ui->frame11);
    Hbox6->addWidget(ui->frame12);
    Vbox->addItem(Hbox6);

    QHBoxLayout * Hbox7 = new QHBoxLayout(this);
    Hbox7->addWidget(ui->frame13);
    Hbox7->addWidget(ui->frame14);
    Vbox->addItem(Hbox7);

    QHBoxLayout * Hbox8 = new QHBoxLayout(this);
    Hbox8->addWidget(ui->frame15);
    //Hbox5->addWidget(ui->frame16);
    Vbox->addItem(Hbox8);

    ui->scrollAreaWidgetContents->setLayout(Vbox);

    QFile file(filename1);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"1";
    }
    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// slots for respective frames
void MainWindow::on_frame1_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/frame1.png";
    }
    file.close();
}

void MainWindow::on_frame2_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/frame2.png";
    }
    file.close();
}

void MainWindow::on_frame3_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/frame1.png";
    }
    file.close();
}

void MainWindow::on_frame4_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/lensframe1.png";
    }
    file.close();
}

void MainWindow::on_frame5_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/sunglass1.png";
    }
    file.close();
}

void MainWindow::on_frame6_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/frame4.png";
    }
    file.close();
}

void MainWindow::on_frame7_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/sunglass3.png";
    }
    file.close();
}

void MainWindow::on_frame8_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/sunglass4.png";
    }
    file.close();
}

void MainWindow::on_frame9_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/sunglass5.png";
    }
    file.close();
}

void MainWindow::on_frame10_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/style1.png";
    }
    file.close();
}

void MainWindow::on_frame11_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/style2.png";
    }
    file.close();
}

void MainWindow::on_frame12_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/style3.png";
    }
    file.close();
}

void MainWindow::on_frame13_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/style4.png";
    }
    file.close();
}

void MainWindow::on_frame14_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/style5.png";
    }
    file.close();
}

void MainWindow::on_frame15_clicked()
{
    QFile file(filename);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<"../frame/style6.png";
    }
    file.close();
}

// function to write slider value into gamma.txt file
void MainWindow::on_horizontalSlider_valueChanged(int value1)
{
    float value = (float)value1/100;
    if( value1 == 0 ) value = 0.1;

    QFile file(filename1);
    file.resize(file.pos());
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream <<value;
    }
    file.close();
    //cout<<"Value "<<value<<endl;
}
