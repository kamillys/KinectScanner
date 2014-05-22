#include "RenderWidget.h"
#include <QMessageBox>
#include <QMouseEvent>

#include "DepthScanner.h"
#include <QDebug>

#define GL_CHECK() \
	if ((err = glGetError()) != GL_NO_ERROR) { \
        cerr << "OpenGL error: " << err << endl; \
    }

RenderWidget::RenderWidget(QWidget* parent)
	: QGLWidget(parent),
    kinect(nullptr),
	logger(nullptr)
{
	setMouseTracking(true);
}


RenderWidget::~RenderWidget(void)
{
}

void RenderWidget::setDevice(KinectDevice* dev)
{
	if (kinect)
	{
		disconnect(kinect, SIGNAL(updated()), this, SLOT(perform_kinect()));
		disconnect(kinect, SIGNAL(repaint()), this, SLOT(repaint()));
	}
	kinect = dev;
	connect(kinect, SIGNAL(updated()), this, SLOT(perform_kinect()));
	connect(kinect, SIGNAL(repaint()), this, SLOT(perform_kinect()));
}

void RenderWidget::perform_kinect()
{
	this->updateGL();
}

static const void build_axis(GLModel* model, int i)
{
	const QVector3D axis[] = { QVector3D(1,0,0), QVector3D(0,1,0), QVector3D(0,0,1) };
	const QVector3D right[] = { QVector3D(0,1,0), QVector3D(0,0,1), QVector3D(1,0,0) };
	const QVector3D normals[] = { QVector3D(0,0,1), QVector3D(1,0,0), QVector3D(0,1,0) };
	model->vertices << QVector3D(0,0,0) << 5*axis[i] << right[i];
	model->normals << normals[i] << normals[i] << normals[i];
}

void RenderWidget::initializeGL()
{
	initializeGLFunctions(context());
	logger = new QOpenGLDebugLogger(this);
	logger->initialize();

	alfa = 0;
	beta = 0;
	dist = 20;

	loadShader(shader,
	"precision highp float;\n"
	"uniform mat4 MVP;\n"
	"attribute vec3 pos;\n"
	"void main(void)\n"
	"{\n"
	"  gl_Position = MVP * vec4(pos, 1);\n"
	"  gl_PointSize = 2.0;\n"
	"}\n",
	"precision highp float;\n"
	"uniform vec4 Color;\n"
	"void main(void)\n"
	"{\n"
	"  gl_FragColor = Color;"
	"}\n");

	for (int i=0;i<3;++i)
	{
		models.push_back(new GLModel(this));
		build_axis(models[i], i);
	}
	models[0]->color = QColor(255,0,0);
	models[1]->color = QColor(0,255,0);
	models[2]->color = QColor(0,0,255);

	
	model.color = QColor(255, 0, 255);
	for (int i=0;i<10;++i)
		model.vertices<<QVector3D(i,0,0)<<QVector3D(i,1,0)<<QVector3D(i+1,1,0);
}

void RenderWidget::resizeGL(int w, int h)
{
	if (h==0) 
		h = 1;
	glViewport(0, 0, w, h);
	pMatrix.setToIdentity();
	pMatrix.perspective(60.0, (float) w/(float) h, 0.1f,100000.0f);
}

void RenderWidget::paintGL()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	vMatrix.setToIdentity();
	//vMatrix.lookAt(QVector3D(10, 10, 10), QVector3D(0, 0, 0), QVector3D(0, 0, 1));
	vMatrix.lookAt(20*QVector3D(sin(beta), sin(alfa)*cos(beta), cos(alfa)*cos(beta)), QVector3D(0, 0, 0), QVector3D(-1, 0, 0));

	QMatrix4x4 VP = pMatrix * vMatrix;
	
	for (int i=0;i<models.size(); ++i)
		models[i]->Render(VP, this, &shader);
	
	model.Render(VP, this, &shader);

	GLModel* models = DepthScanner::getModel();
	for (int i=0; i<DepthScanner::modelCount(); ++i)
		models[i].Render(VP, this, &shader);

	if (logger->isLogging()) {
	QList<QOpenGLDebugMessage> messages = logger->loggedMessages();
	foreach (const QOpenGLDebugMessage &message, messages)
	    qDebug() << message;
	} else {
		GLenum e = glGetError();
		if (e != GL_NO_ERROR)
		{
			qDebug() << "GL Error!" << e;
		}
	}
}

#define qglassert(p, v) \
{ \
	bool b = (v); \
	if (!b) \
	{ \
	    QString l = (p).log(); \
	 	qDebug() << l;\
		QMessageBox::critical(this, "Error", l); \
	} \
	assert(b); \
}

void RenderWidget::loadShader(QGLShaderProgram& program, QString vs, QString fs)
{
	qglassert(program, (program.addShaderFromSourceCode(QGLShader::Vertex, vs)));
	qglassert(program, (program.addShaderFromSourceCode(QGLShader::Fragment, fs)));
	qglassert(program, (program.link()));
}


void RenderWidget::mouseMoveEvent(QMouseEvent * ev)
{
	if (!(ev->buttons() & Qt::LeftButton))
	{
		return;
	}
	QPoint dp = mousePt - ev->pos();
	mousePt = ev->pos();

	QPointF fdp (dp);
	fdp *= 0.02;
	
	QString dd = QString("( %1 , %2 )").arg(fdp.x()).arg(fdp.y());
	//QMessageBox::critical(this, "", dd);
	window()->setWindowTitle(dd);
	alfa += fdp.x();
	//vMatrix.translate(fdp.x(), fdp.y(), 0);

	updateGL();
}

void RenderWidget::mousePressEvent(QMouseEvent * ev)
{
	mousePt = ev->pos();
}

void RenderWidget::mouseReleaseEvent(QMouseEvent * ev)
{
	mousePt = ev->pos();
}

void RenderWidget::wheelEvent(QWheelEvent* ev)
{
	ev->delta();
}
