#include "kinectdevice.h"
#include <QMessageBox>
#include <QDebug>

using namespace cv;

static const std::string depthWindow = "dasdf";
static const std::string colorWindow = "casdf";

KinectDevice::KinectDevice()
    : sensor(nullptr),
      colorStream(INVALID_HANDLE_VALUE),
      depthStream(INVALID_HANDLE_VALUE)
{
    if (!initializeKinect())
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot initialize MS Kinect!");
        msgBox.exec();
        throw 1;
    }
}

KinectDevice::~KinectDevice()
{
    if (sensor)
    {
        sensor->NuiShutdown();
        sensor->Release();
    }
}


Mat KinectDevice::updateDepth()
{
	Mat retval;
	NUI_IMAGE_FRAME frame;
	if (FAILED(sensor->NuiImageStreamGetNextFrame(depthStream,
		0, &frame)))
		return retval;
	NUI_LOCKED_RECT rect;
	int len = frame.pFrameTexture->BufferLen();
	frame.pFrameTexture->LockRect(0, &rect, nullptr, 0);
	if (rect.Pitch != 0)
	{
		assert(640 * 480 * 2 == rect.size);
		Mat img(480, 640, CV_16UC1);
		memcpy(img.ptr<short>(), rect.pBits, rect.size);
		img /= 8;
		retval = img;
	}
	frame.pFrameTexture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(depthStream, &frame);
	return retval;
}

Mat KinectDevice::updateColor()
{
	Mat retval;
	NUI_IMAGE_FRAME frame;
	if (FAILED(sensor->NuiImageStreamGetNextFrame(colorStream,
		0, &frame)))
		return retval;
	NUI_LOCKED_RECT rect;
	frame.pFrameTexture->LockRect(0, &rect, nullptr, 0);
	if (rect.Pitch != 0)
	{
		assert(640 * 480 * 4 == rect.size);
		Mat img(480, 640, CV_8UC4);
		memcpy(img.ptr<Vec4b>(), rect.pBits, rect.size);
		cvtColor(img, img, CV_BGRA2BGR);
		retval = img;
	}
	frame.pFrameTexture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(colorStream, &frame);
	return retval;
}

static QRgb depth_to_color(short d)
{
	if (d == 0x1FFF)
		return qRgb(0,0,0);
	d = 0xFF * ((double) d / (double)0x1FFF);
	return qRgb(d, d, d);
	/*
	static short t_gamma[2048];
	static int tgam = [&]() {
		for (int i=0; i<2048; i++) {
			float v = i/2048.0;
			v = powf(v, 3)* 6;
			t_gamma[i] = v*6*256;
		}
		return 0;
	}();
	
	//d = d & 0x7FF;
	int pval = t_gamma[d];
	int lb = pval & 0x0ff;
	int r, g, b;
	switch (pval>>8) {
		case 0: r = 255;    g = 255-lb; b = 255-lb; break;
		case 1: r = 255;    g = lb;     b = 0; break;
		case 2: r = 255-lb; g = 255;    b = 0; break;
		case 3: r = 0;      g = 255;    b = lb; break;
		case 4: r = 0;      g = 255-lb; b = 255; break;
		case 5: r = 0;      g = 0;      b = 255-lb; break;
		default: r = 0;     g = 0;      b = 0;
		break;
	}

	return qRgb(r,g,b);*/
}


void KinectDevice::update()
{
	Mat _color = updateColor();
	Mat _depth = updateDepth();
    QImage qimg;
	const bool validClr = !_color.empty(),
		validDepth = !_depth.empty();

	if (!validClr || !validDepth)
		return;

	color = _color;
	depth = _depth;
	
	lastColorImage = QImage(2*640, 480, QImage::Format_RGB32);
	for (int j=0;j<color.rows;++j)
	{
		QRgb* scanline = (uint*)lastColorImage.scanLine(j);
		for (int i=0;i<color.cols;++i)
		{
			Vec3b c = color.at<cv::Vec3b>(j, i);
			//lastColorImage.setPixel(i, j, qRgb(c[2], c[1], c[0]));
			scanline[i] = qRgb(c[2], c[1], c[0]);
		}
		for (int i=0;i<640;++i)
		{
			short d = depth.at<short>(j, i);
			//lastColorImage.setPixel(640+i, j, qRgb(c[0], c[1], c[2]));
			scanline[640+i] = depth_to_color(d);
		}
	}
	//imshow(colorWindow, color);
	//imshow(depthWindow, depth);

    emit updated();
}

cv::Mat KinectDevice::getColorImage()
{
	return color;
}

cv::Mat KinectDevice::getDepthImage()
{
	return depth;
}

QImage KinectDevice::getQImage()
{
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
    if (FAILED(sensor->NuiInitialize( NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR)))
        return false;
    if (FAILED(sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,
        NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &colorStream)))
        return false;
    if (FAILED(sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,
        NUI_IMAGE_RESOLUTION_640x480,
        NUI_IMAGE_STREAM_FLAG_DISTINCT_OVERFLOW_DEPTH_VALUES
		/*NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE*/,
        2, NULL, &depthStream)))
        return false;
    return true;
}
