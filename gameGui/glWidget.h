#pragma once

#include <QtOpenGL>
#include "model.h"

class GlWidget : public QGLWidget {
  Q_OBJECT
public:
  GlWidget(QWidget *parent = 0);
  ~GlWidget();
  QSize sizeHint() const;

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  QGLShaderProgram lightingShaderProgram;
  QGLShaderProgram colouringShaderProgram;
  QGLShaderProgram simpleShaderProgram;

  QVector<QVector3D> landVertices;
  QVector<QVector3D> spotlightVertices;
  QVector<QVector3D> spotlightColours;

  void drawLand(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix);
  void drawModel(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix, QVector3D lightPosition);

  QMatrix4x4 pMatrix;

  QVector<Model*> cubes;

  double alpha;
  double beta;
  double distance;
  QPoint lastMousePosition;
};
