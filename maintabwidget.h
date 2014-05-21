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
private:
	Ui::MainTabWidget* ui;
};

