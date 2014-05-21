#include "DepthScanner.h"


namespace DepthScanner
{

static KinectDevice* kinect = nullptr;
static GLModel objectModel;
static bool is_Initialized = false;
static const int S = 128;
static const double dS = static_cast<double>(S);

static void initialize()
{
	if (is_Initialized)
		return;
	is_Initialized = true;

	objectModel.color = QColor(0, 255, 0);
	objectModel.modelMatrix.scale(5);

	int k = 0;

	for (int i=0;i<S;++i)
	for (int j=0;j<S;++j)
	//for (int k=0;k<S;++k)
	objectModel.vertices << QVector3D(i/dS, j/dS, k/dS);
}

void setKinectDevice(KinectDevice* d)
{
	kinect = d;
	initialize();
}

GLModel* getModel()
{
	initialize();
	return &objectModel;
}

void updateModel()
{
	cv::Mat depth = kinect->getDepthImage();
	if (depth.empty())
		return;
	for (int i=0;i<S;++i)
	for (int j=0;j<S;++j)
	{
		int X = depth.rows * (i / dS);
		int Y = depth.cols * (j / dS);
		QVector3D& v = objectModel.vertices[i*S+j];
		short val = depth.at<short>(X, Y);
		v.setZ(val / 20000.0);
	}
}


}
