#pragma once
#include <QWidget>
#include "kinectdevice.h"

namespace Ui {
class MainTabWidget;
}

class MainTabWidget : public QWidget
{
    Q_OBJECT
public:
	MainTabWidget(QWidget *parent = 0);
	~MainTabWidget(void);
	
    void setDevice(KinectDevice* dev);
private slots:
	void on_resetButton_clicked();
	void on_shotButton_clicked();
	void on_angleBox_valueChanged(double a);
	void on_distanceBox_valueChanged(double d);
private:
	Ui::MainTabWidget* ui;
};

