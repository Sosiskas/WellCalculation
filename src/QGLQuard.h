#pragma once
#include <QWidget>
#include <QOpenGLWidget>
#include "Resource.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <d3d8types.h>
// ====================================================
class OGL : public QOpenGLWidget {
public:
	void setData(ChartingData coord);
	void setMinimalCurvatureMethodData(ChartingData coord);
protected:
    void initializeGL( );
    void resizeGL (int nWidth, int nHeight);
    void paintGL ( );

    QSize minimumSizeHint() const;
//    QSize sizeHint();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    OGL(QWidget* pwgt = 0);

public slots:
    void rotate(double x);
    void setXRot(int angle);
    void setYRot(int angle);
    void setZRot(int angle);

private:
    void draw();
    void draw_Axis();
    void renderText(D3DVECTOR &textPosWorld, QString text);
    inline int project(GLdouble objx, GLdouble objy, GLdouble objz,
                   const GLdouble model[16], const GLdouble proj[16],
                   const GLint viewport[4],
                   GLdouble * winx, GLdouble * winy, GLdouble * winz);
    inline void transformPoint(GLdouble out[4], const GLdouble m[16], const GLdouble in[4]);
    int xRot;
    int yRot;
    int zRot;
	ChartingData _coord;
	ChartingData _minimalCurvatureMethodData;
    double zoomScale;
    QPoint lastPos;
};
