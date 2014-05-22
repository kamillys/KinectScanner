#include "DepthScanner.h"


namespace DepthScanner
{

static KinectDevice* kinect = nullptr;
static GLModel* depthPoints;
static const int objectCount = 2;
static bool is_Initialized = false;
static const int S = 128;
static const double dS = static_cast<double>(S);

static int distanceToObject = 0;

static int* voxels;

static void initialize()
{
	if (is_Initialized)
		return;
	is_Initialized = true;

	voxels = new int [S*S*S];
	memset(voxels, 1, sizeof(int) * S*S*S);

	depthPoints = new GLModel[objectCount];
	depthPoints->type = GLModel::POINTS;
	depthPoints->color = QColor(0, 255, 0);

	for (int i=0;i<S;++i) for (int j=0;j<S;++j)
	depthPoints->vertices << QVector3D(i/dS, j/dS, 0);

	depthPoints[1].type = GLModel::POINTS;
	depthPoints[1].color = QColor(0, 128, 255);

	for (int i=0;i<objectCount;++i)
	{
		depthPoints[i].modelMatrix.setToIdentity();
		depthPoints[i].modelMatrix.translate(0, 0, -4);
		depthPoints[i].modelMatrix.scale(8);
	}

	depthPoints[1].vertices
		<< QVector3D(-1, -1, -1)
		<< QVector3D( 1, -1, -1)
		<< QVector3D(-1,  1, -1)
		<< QVector3D( 1,  1, -1)
		<< QVector3D(-1, -1,  1)
		<< QVector3D( 1, -1,  1)
		<< QVector3D(-1,  1,  1)
		<< QVector3D( 1,  1,  1);
}

void setKinectDevice(KinectDevice* d)
{
	kinect = d;
	initialize();
}

int modelCount()
{
	return objectCount;
}

GLModel* getModel()
{
	initialize();
	return depthPoints;
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
		QVector3D& v = depthPoints->vertices[i*S+j];
		short val = depth.at<short>(X, Y);

		Vector4 pos = NuiTransformDepthImageToSkeleton(X, Y, val << 3, NUI_IMAGE_RESOLUTION_640x480);
		pos.x = pos.x/pos.w;
		pos.y = pos.y/pos.w;
		pos.z = pos.z/pos.w;
		
		v.setX(pos.x);
		v.setY(pos.y);
		v.setZ(pos.z);
	}

	//depthPoints->modelMatrix.translate(0, 0, -4);
	//depthPoints->modelMatrix.scale(8);
}


}
