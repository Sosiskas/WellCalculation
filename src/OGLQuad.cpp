#include "QGLQuard.h"
#include <QGLWidget>
#include <QtOpenGL>
#include <GL/gl.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGL>
#include <QGLFormat>
#include <QGLContext>
#include <QGLColormap>
#include <QGLPixelBuffer>
#include <QGLFunctions>
#include <QtGlobal>
#include <d3d8types.h>


OGL::OGL(QWidget* pwgt/*= 0*/) : QOpenGLWidget(pwgt)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    zoomScale = 1.0;
	_coord.push_back(Coordinates());
}
void OGL::setData(ChartingData coord)
{
	_coord = coord;
	draw_Axis();
	draw();
}
void OGL::setMinimalCurvatureMethodData(ChartingData coord)
{
	_minimalCurvatureMethodData = coord;
}
void OGL::initializeGL()
{
       glEnable(GL_DEPTH_TEST);
       glEnable(GL_CULL_FACE);
       glShadeModel(GL_SMOOTH);
}

void OGL::resizeGL(int nWidth, int nHeight)
{
    int side = qMin(nWidth, nHeight);
    glViewport((nWidth - side) / 2, (nHeight - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);

}

void OGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glScalef(zoomScale,zoomScale,zoomScale);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    draw_Axis();
    draw();
}

QSize OGL::minimumSizeHint() const
{
    return QSize(50,50);
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void OGL::setXRot(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        update();
    }
}

void OGL::setYRot(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        update();
    }
}

void OGL::setZRot(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        update();
    }
}

void OGL::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void OGL::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRot(xRot + 8 * dy);
        setYRot(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRot(xRot + 8 * dy);
        setZRot(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void OGL::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
    zoomScale *=1.1;
    }
    else
    {
    zoomScale /=1.1;
    }
    update();
}

void OGL::draw()
{
    glPointSize(5);
	if (_coord.size() != 0)
	{
		glBegin(GL_LINE_STRIP);
		glColor4f(0.35f, 0.90f, 1.00f, 1.00f);
		for (int i = 0; i < _coord.size(); i++)
		{
			glVertex3f(_coord[i].x, _coord[i].y, _coord[i].z);
		}
		glEnd();
	}
	if (_minimalCurvatureMethodData.size() != 0)
	{
		glBegin(GL_LINE_STRIP);
		glColor4f(0.00f, 1.00f, 0.00f, 1.00f);
		for (int i = 0; i < _coord.size(); i++)
		{
			glVertex3f(_minimalCurvatureMethodData[i].x, _minimalCurvatureMethodData[i].y, _minimalCurvatureMethodData[i].z);
		}
		glEnd();
	}
	
}

void OGL::draw_Axis()
{
    glLineWidth(3.0f);
    glColor4f(1.00f,1.00f,1.00f,1.00f);
    glBegin(GL_LINES);
    glVertex3f(_coord[_coord.size()-1].x,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glEnd();
    D3DVECTOR vecX = { _coord[_coord.size() - 1].x,0,0};
    renderText(vecX,"X");

    glBegin(GL_LINES);
    glVertex3f(0.0f, _coord[_coord.size() - 1].y,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glEnd();
    D3DVECTOR vecY = {0,_coord[_coord.size() - 1].y,0};
    renderText(vecY,"Y");

    glBegin(GL_LINES);
    glVertex3f(0.0f,0.0f, _coord[_coord.size() - 1].z);
    glVertex3f(0.0f,0.0f,0.0f);
    glEnd();
    D3DVECTOR vecZ = {0,0,_coord[_coord.size() - 1].z};
    renderText(vecZ,"Z");

}

void OGL::renderText(D3DVECTOR &textPosWorld, QString text)
{
//    int width = this->width();
    int height = this->height();

    GLdouble model[4][4], proj[4][4];
    GLint view[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, &model[0][0]);
    glGetDoublev(GL_PROJECTION_MATRIX, &proj[0][0]);
    glGetIntegerv(GL_VIEWPORT, &view[0]);
    GLdouble textPosX = 0, textPosY = 0, textPosZ = 0;

    project(textPosWorld.x, textPosWorld.y, textPosWorld.z,
                &model[0][0], &proj[0][0], &view[0],
                &textPosX, &textPosY, &textPosZ);

    textPosY = height - textPosY; // y is inverted

    QPainter painter(this);
    painter.setPen(Qt::yellow);
    painter.setFont(QFont("Helvetica", 8));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawText(textPosX, textPosY, text); // z = pointT4.z + distOverOp / 4
    painter.end();
}

inline int OGL::project(GLdouble objx, GLdouble objy, GLdouble objz,
    const GLdouble model[16], const GLdouble proj[16],
    const GLint viewport[4],
    GLdouble * winx, GLdouble * winy, GLdouble * winz)
{
    GLdouble in[4], out[4];

    in[0] = objx;
    in[1] = objy;
    in[2] = objz;
    in[3] = 1.0;
    transformPoint(out, model, in);
    transformPoint(in, proj, out);

    if (in[3] == 0.0)
        return GL_FALSE;

    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];

    *winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;
    *winy = viewport[1] + (1 + in[1]) * viewport[3] / 2;

    *winz = (1 + in[2]) / 2;
    return GL_TRUE;
}

inline void OGL::transformPoint(GLdouble out[4], const GLdouble m[16], const GLdouble in[4])
{
#define M(row,col)  m[col*4+row]
    out[0] =
        M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
    out[1] =
        M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
    out[2] =
        M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
    out[3] =
        M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}
