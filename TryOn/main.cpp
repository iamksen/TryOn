#include "mainwindow.h"
#include <QApplication>
#include "message.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.move(50,10); //initial positino of window
    return a.exec();
}
