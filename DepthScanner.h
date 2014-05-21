#pragma once

#include "kinectdevice.h"
#include "GLModel.h"

namespace DepthScanner
{

void setKinectDevice(KinectDevice* d);
void updateModel();
GLModel* getModel();

}

