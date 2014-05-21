#include "GLModel.h"


GLModel::GLModel(QObject* parent)
	: QObject(parent)
{
}


GLModel::~GLModel(void)
{
}

void GLModel::Render(const QMatrix4x4& MV, QGLFunctions* gl, QGLShaderProgram* shader)
{
	static const char* vertices_name = "pos";
	QMatrix4x4 MVP = MV * modelMatrix;
	shader->bind();
	shader->setUniformValue( "MVP" , MVP);
	shader->setUniformValue( "Color", color);
	shader->setAttributeArray( vertices_name, vertices.constData());
	shader->enableAttributeArray( vertices_name );
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	shader->disableAttributeArray( vertices_name ); 
	shader->release();
}

void GLModel::RenderPoints(const QMatrix4x4& MV, QGLFunctions* gl, QGLShaderProgram* shader)
{
	static const char* vertices_name = "pos";
	QMatrix4x4 MVP = MV * modelMatrix;
	shader->bind();
	shader->setUniformValue( "MVP" , MVP);
	shader->setUniformValue( "Color", color);
	shader->setAttributeArray( vertices_name, vertices.constData());
	shader->enableAttributeArray( vertices_name );
	glDrawArrays(GL_POINTS, 0, vertices.size());
	shader->disableAttributeArray( vertices_name ); 
	shader->release();
}