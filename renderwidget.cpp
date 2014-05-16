#include "RenderWidget.h"


RenderWidget::RenderWidget(QWidget* parent)
	: QGLWidget(parent)
{
}


RenderWidget::~RenderWidget(void)
{
}

void RenderWidget::initializeGL()
{
	initializeGLFunctions(context());
}

void RenderWidget::resizeGL()
{
}

void RenderWidget::paintGL()
{
	glClearColor(1,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
}
