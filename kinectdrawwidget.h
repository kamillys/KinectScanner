#ifndef KINECTDRAWWIDGET_H
#define KINECTDRAWWIDGET_H

#include <QWidget>
#include "kinectdevice.h"

class KinectDrawWidget : public QWidget
{
    Q_OBJECT
    KinectDevice* kinect;
public:
    explicit KinectDrawWidget(QWidget *parent = 0);
    void setDevice(KinectDevice* dev) { kinect = dev; repaint(); }
protected:
    void paintEvent(QPaintEvent*);
};

#endif // KINECTDRAWWIDGET_H
