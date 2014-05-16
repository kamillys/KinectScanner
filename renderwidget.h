#pragma once
#include <QGLWidget>
#include <QGLFunctions>

class RenderWidget : public QGLWidget, protected QGLFunctions
{
	Q_OBJECT
public:
	RenderWidget(QWidget* parent = 0);
	~RenderWidget(void);

protected:
	void initializeGL();
	void resizeGL();
	void paintGL();
};

