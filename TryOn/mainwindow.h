#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_frame1_clicked();

    void on_frame2_clicked();

    void on_frame3_clicked();

    void on_frame4_clicked();

    void on_frame5_clicked();

    void on_frame6_clicked();

    void on_frame7_clicked();

    void on_frame8_clicked();

    void on_frame9_clicked();

    void on_frame10_clicked();

    void on_frame11_clicked();

    void on_frame12_clicked();

    void on_frame13_clicked();

    void on_frame14_clicked();

    void on_frame15_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
