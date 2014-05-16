#pragma once
#include <QWidget>

namespace Ui {
class MainTabWidget;
}

class MainTabWidget : public QWidget
{
    Q_OBJECT
public:
	MainTabWidget(QWidget *parent = 0);
	~MainTabWidget(void);

private:
	Ui::MainTabWidget* ui;
};

