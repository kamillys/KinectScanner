#ifndef KINECTDEVICE_H
#define KINECTDEVICE_H

#include <Windows.h>
#include <NuiApi.h>

#include <functional>
#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

#define FAILED(hr) (((HRESULT)(hr)) < 0)

class KinectDevice : public QObject
{
	Q_OBJECT
    INuiSensor* sensor;
    HANDLE colorStream;
    HANDLE depthStream;
	cv::Mat color;
	cv::Mat depth;

    QImage lastColorImage;

	cv::Mat updateDepth();
	cv::Mat updateColor();

    bool initializeKinect();
public:
    KinectDevice();
    ~KinectDevice();

	cv::Mat getColorImage();
	cv::Mat getDepthImage();

	QImage getQImage();
	bool valid() const {
		return nullptr != sensor;
	}
	
public slots:
    void update();
	void do_repaint() {
		emit repaint();
	}
signals:
    void updated();
	void repaint();
};

#endif // KINECTDEVICE_H
