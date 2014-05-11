#ifndef KINECTDEVICE_H
#define KINECTDEVICE_H

#include <Windows.h>
#include <NuiApi.h>

#include <QImage>

#define FAILED(hr) (((HRESULT)(hr)) < 0)

class KinectDevice
{
    INuiSensor* sensor;
    HANDLE colorStream;
    HANDLE depthStream;

    QImage lastColorImage;
public:
    KinectDevice();
    ~KinectDevice();

    QImage getColorImage();
private:
    bool initializeKinect();
};

#endif // KINECTDEVICE_H
