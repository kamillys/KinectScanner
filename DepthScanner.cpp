#include "DepthScanner.h"


namespace DepthScanner
{
	
static void computeVoxelMesh();

static KinectDevice* kinect = nullptr;
static QMatrix4x4 GlobalMatrix;
static GLModel* depthPoints;
static const int objectCount = 3;
static bool is_Initialized = false;
static const int S = 32;
static const int DepthS = 128;
static const double dS = static_cast<double>(S);
static const double dDepthS = static_cast<double>(DepthS);
static double distanceToObject = 0;
static double rotationAngle = 0;
static int* voxels;

static void initialize()
{
	if (is_Initialized)
		return;
	is_Initialized = true;

	voxels = new int [S*S*S];
	
	GlobalMatrix.translate(0, 0, -4);
	GlobalMatrix.scale(8);

	depthPoints = new GLModel[objectCount];
	depthPoints->type = GLModel::POINTS;
	depthPoints->color = QColor(0, 255, 0);

	for (int i=0;i<DepthS;++i) for (int j=0;j<DepthS;++j)
	depthPoints->vertices << QVector3D(i/dS, j/dS, 0);

	depthPoints[1].type = GLModel::POINTS;
	depthPoints[1].color = QColor(0, 128, 255);

	depthPoints[2].color = QColor(255, 255, 255);
	depthPoints[2].type = GLModel::POINTS;
	depthPoints[2].vertices
		<< QVector3D( 0, 0, 0)
		<< QVector3D(-0.5, 0, 0)
		<< QVector3D( 0.5, 0, 0)
		<< QVector3D( 0,-0.5, 0)
		<< QVector3D( 0, 0.5, 0)
		<< QVector3D( 0, 0,-0.5)
		<< QVector3D( 0, 0, 0.5);

	for (int i=0;i<objectCount;++i)
		depthPoints[i].modelMatrix = GlobalMatrix;
	
	resetVoxels();
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
	for (int i=0;i<DepthS;++i)
	for (int j=0;j<DepthS;++j)
	{
		int X = depth.rows * (i / dDepthS);
		int Y = depth.cols * (j / dDepthS);
		QVector3D& v = depthPoints->vertices[i*dDepthS+j];
		short val = depth.at<short>(X, Y);

		Vector4 pos = NuiTransformDepthImageToSkeleton(X, Y, val << 3, NUI_IMAGE_RESOLUTION_640x480);
		pos.x = pos.x/pos.w;
		pos.y = pos.y/pos.w;
		pos.z = pos.z/pos.w;
		
		v.setX(pos.x);
		v.setY(pos.y);
		v.setZ(pos.z);
	}
}

void resetVoxels()
{
	memset(voxels, 0xFF, sizeof(int) * S*S*S);
	computeVoxelMesh();
}

static void computeVoxelMesh()
{
	depthPoints[1].vertices.clear();
	static const int Sm = S - 1;
	static const double dSm = S - 1;
	
	for (int i=0;i<S;++i)
	for (int j=0;j<S;++j)
	for (int k=0;k<S;++k)
	{
		if (voxels[i*S*S+j*S+k] != 0)
		{
			double di = 2 * i / dSm;
			double dj = 2 * j / dSm;
			double dk = 2 * k / dSm;
			depthPoints[1].vertices
				<< QVector3D(-1 + di, -1 + dj, -1 + dk);
		}
	}
}

static void computeRotations()
{
	depthPoints[1].modelMatrix = GlobalMatrix;
	depthPoints[1].modelMatrix.translate(0, 0, distanceToObject);
	depthPoints[1].modelMatrix.rotate(rotationAngle, -1, 0, 0);
}

void setDistanceToObject(double d)
{
	distanceToObject = d;
	computeRotations();
}

void setObjectRotationAngle(double a)
{
	rotationAngle = a;
	computeRotations();
}

void goShotObject()
{
	//TODO: Perform vertex clipping
	for (int i=0;i<S;++i)
	for (int j=0;j<S;++j)
	for (int k=0;k<S;++k)
	{
		if ( sqrt(i*i+j*j+k*k) > 25)
			voxels[i*S*S+j*S+k] = 0;
	}

	computeVoxelMesh();
}

}
