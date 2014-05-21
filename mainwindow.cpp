#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DepthScanner.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	if (!kinect.valid())
		setWindowTitle("NO KINECT DETECTED!");
    ui->kinect_tab->setDevice(&kinect);
	ui->main_tab->setDevice(&kinect);
	DepthScanner::setKinectDevice(&kinect);

	connect(&timer, SIGNAL(timeout()), this, SLOT(updateKinect()));
	timer.start(40);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateKinect()
{
	kinect.update();
	DepthScanner::updateModel();
	kinect.do_repaint();
}
