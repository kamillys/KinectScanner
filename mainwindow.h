#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

#include "kinectdevice.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    KinectDevice kinect;
	QTimer timer;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	void updateKinect();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
