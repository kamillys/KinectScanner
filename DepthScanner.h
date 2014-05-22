#pragma once

#include "kinectdevice.h"
#include "GLModel.h"

namespace DepthScanner
{

void setKinectDevice(KinectDevice* d);
void updateModel();
int modelCount();
GLModel* getModel();
void resetVoxels();
void setDistanceToObject(double d);
void setObjectRotationAngle(double a);
void goShotObject();

}
