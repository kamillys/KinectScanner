#include "MainTabWidget.h"
#include "ui_maintabwidget.h"

MainTabWidget::MainTabWidget(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::MainTabWidget)
{
	ui->setupUi(this);
}


MainTabWidget::~MainTabWidget(void)
{
}
