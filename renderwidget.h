#pragma once
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QOpenGLDebugLogger>

#include "kinectdevice.h"
#include "GLModel.h"

class RenderWidget : public QGLWidget, protected QGLFunctions
{
	Q_OBJECT
    KinectDevice* kinect;
	QOpenGLDebugLogger* logger;
	GLModel model;
	QVector<GLModel*> models;
public:
	RenderWidget(QWidget* parent = 0);
	~RenderWidget(void);
	
    void setDevice(KinectDevice* dev);
protected slots:
	void perform_kinect();
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	
	void mouseMoveEvent(QMouseEvent * ev);
	void mousePressEvent(QMouseEvent * ev);
	void mouseReleaseEvent(QMouseEvent * ev);
	void wheelEvent(QWheelEvent* ev);

	double dist;
	double alfa;
	double beta;

	QPoint mousePt;
	QMatrix4x4 pMatrix;
	QMatrix4x4 vMatrix;
	QGLShaderProgram shader;
	void loadShader(QGLShaderProgram& program, QString vs, QString fs);
};
