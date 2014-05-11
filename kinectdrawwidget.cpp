#include "kinectdrawwidget.h"
#include <QPainter>
#include <QTimer>

KinectDrawWidget::KinectDrawWidget(QWidget *parent) :
    QWidget(parent),
    kinect(nullptr)
{
    QTimer::singleShot(20, this, SLOT(repaint()));
}

void KinectDrawWidget::paintEvent(QPaintEvent *)
{
    if (!kinect)
        return;
    QTimer::singleShot(20, this, SLOT(repaint()));
    QPainter p(this);
    p.drawImage(0, 0, kinect->getColorImage());
}
