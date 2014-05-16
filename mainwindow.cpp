#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->kinect_tab->setDevice(&kinect);
	connect(&timer, SIGNAL(timeout()), &kinect, SLOT(update()));
	timer.start(40);
}

MainWindow::~MainWindow()
{
    delete ui;
}
