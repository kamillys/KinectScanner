#pragma once

#include <QObject>
#include <QGLFunctions>
#include <QGLShaderProgram>

class GLModel : public QObject
{
	Q_OBJECT
public:
	GLModel(QObject* parent = nullptr);
	virtual ~GLModel(void);
	
	void Render(const QMatrix4x4& VP, QGLFunctions* gl, QGLShaderProgram* shader);
	
	enum {
		POINTS,
		TRIANGLES
	} type;

	QColor color;
	QMatrix4x4 modelMatrix;
	QVector<QVector3D> vertices;
	QVector<QVector3D> normals; // TO BE USED. soon...
};
