#pragma once

#include <QtOpenGL>
#include "model.h"
#include "graph.h"
#include "building.h"

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

  void HandleLeftClick(int mouseX, int mouseY);
  QMatrix4x4 projectionMatrix;
  QList<Road*> roads;

  double alpha;
  double beta;
  double distance;
  QPoint lastMousePosition;

  double moveUp, moveLeft;

  bool panUp, panDown, panLeft, panRight;

  BuildingFactory buildingFactory;

  QTimer* panTimer;
  QMatrix4x4 mvMatrix;

  QList<QVector3D> buildings;

private slots:
  void updatePan();
};
