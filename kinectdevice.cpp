#include "kinectdevice.h"


KinectDevice::KinectDevice()
    : sensor(nullptr),
      colorStream(INVALID_HANDLE_VALUE),
      depthStream(INVALID_HANDLE_VALUE)
{
    initializeKinect();
}

KinectDevice::~KinectDevice()
{
    if (sensor)
    {
        sensor->NuiShutdown();
        sensor->Release();
    }
}

QImage KinectDevice::getColorImage()
{
    lastColorImage;
    QImage img;
    NUI_IMAGE_FRAME frame;
    if (FAILED(sensor->NuiImageStreamGetNextFrame(colorStream,
        0, &frame)))
        return lastColorImage;
    NUI_LOCKED_RECT rect;
    frame.pFrameTexture->LockRect(0, &rect, nullptr, 0);
    if (rect.Pitch != 0)
    {
        img = QImage(rect.pBits, 640, 480, QImage::Format_RGB32).copy();
    }
    frame.pFrameTexture->UnlockRect(0);
    sensor->NuiImageStreamReleaseFrame(colorStream, &frame);

    if (!img.isNull())
        lastColorImage = img;
    return lastColorImage;
}

bool KinectDevice::initializeKinect()
{
    int numSensors;
    if (FAILED(NuiGetSensorCount(&numSensors)) || numSensors < 1)
        return false;
    INuiSensor * pSensor;

    for (int i = 0; i < numSensors; ++i)
    {
        if (FAILED(NuiCreateSensorByIndex(i, &pSensor)))
            continue;
        if (SUCCEEDED(pSensor->NuiStatus()))
        {
            //POSSIBLE LEAK!
            sensor = pSensor;
            break;
        }
        pSensor->Release();
    }
    if (!sensor)
        return false;
    if (FAILED(sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH|
        NUI_INITIALIZE_FLAG_USES_COLOR)))
        return false;
    if (FAILED(sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,
        NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &colorStream)))
        return false;
    if (FAILED(sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,
        NUI_IMAGE_RESOLUTION_640x480,
        NUI_IMAGE_STREAM_FLAG_DISTINCT_OVERFLOW_DEPTH_VALUES,
        2, NULL, &depthStream)))
        return false;
    return true;
}
