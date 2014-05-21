#include "kinectdrawwidget.h"
#include <QPainter>
#include <QTimer>

KinectDrawWidget::KinectDrawWidget(QWidget *parent) :
    QWidget(parent),
    kinect(nullptr)
{
}

void KinectDrawWidget::paintEvent(QPaintEvent *)
{
    if (!kinect)
        return;
    QPainter p(this);
    p.drawImage(0, 0, kinect->getQImage());
}

void KinectDrawWidget::setDevice(KinectDevice* dev)
{
	if (kinect)
	{
		disconnect(kinect, SIGNAL(updated()), this, SLOT(repaint()));
	}
	kinect = dev;
	connect(kinect, SIGNAL(updated()), this, SLOT(repaint()));
}
