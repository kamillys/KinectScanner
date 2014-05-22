#include "MainTabWidget.h"
#include "ui_maintabwidget.h"
#include "DepthScanner.h"

MainTabWidget::MainTabWidget(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::MainTabWidget)
{
	ui->setupUi(this);
}


MainTabWidget::~MainTabWidget(void)
{
}

void MainTabWidget::setDevice(KinectDevice* dev)
{
	ui->widget->setDevice(dev);
}

void MainTabWidget::on_resetButton_clicked()
{
	DepthScanner::resetVoxels();
}

void MainTabWidget::on_shotButton_clicked()
{
	DepthScanner::goShotObject();
}

void MainTabWidget::on_angleBox_valueChanged(double a)
{
	DepthScanner::setObjectRotationAngle(a);
}

void MainTabWidget::on_distanceBox_valueChanged(double d)
{
	DepthScanner::setDistanceToObject(d);
}
