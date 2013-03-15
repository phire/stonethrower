#include "glWidget.h"
#include "savefile.h"

GlWidget::GlWidget(QWidget *parent)
  : QGLWidget(QGLFormat(/* Additional format options */), parent)
{
  alpha = 25;
  beta= -25;
  distance = 2.5;

  moveUp = 0.0;
  moveLeft = 0.0;

  SaveFile* file = new SaveFile(":/resources/onestreet.st");
  roads = file->GetRoads();

  setMouseTracking(true);

  panUp = panDown = panLeft = panRight = false;
  panTimer = new QTimer(this);
  connect(panTimer, SIGNAL(timeout()), this, SLOT(updatePan()));
}

GlWidget::~GlWidget() {
}

QSize GlWidget::sizeHint() const {
  return QSize(800, 600);
}

void GlWidget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_LINE_SMOOTH);

  qglClearColor(QColor(Qt::black));
  lightingShaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/lightingVertexShader.vsh");
  lightingShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/lightingFragmentShader.fsh");
  lightingShaderProgram.link();

  colouringShaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/vertexShader.vsh");
  colouringShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/fragmentShader.fsh");
  colouringShaderProgram.link();

  simpleShaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/singleColourVertexShader.vsh");
  simpleShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/singleColourFragmentShader.fsh");
  simpleShaderProgram.link();

  landVertices << QVector3D(-1000, 0, -1000) << QVector3D(-1000, 0, 1000) << QVector3D(1000, 0, 1000)
               << QVector3D(1000, 0, 1000) <<  QVector3D(1000, 0, -1000) << QVector3D(-1000, 0, -1000);

  spotlightVertices << QVector3D(   0,    1,    0) << QVector3D(-0.5,    0,  0.5) << QVector3D( 0.5,    0,  0.5) // Front
                    << QVector3D(   0,    1,    0) << QVector3D( 0.5,    0, -0.5) << QVector3D(-0.5,    0, -0.5) // Back
                    << QVector3D(   0,    1,    0) << QVector3D(-0.5,    0, -0.5) << QVector3D(-0.5,    0,  0.5) // Left
                    << QVector3D(   0,    1,    0) << QVector3D( 0.5,    0,  0.5) << QVector3D( 0.5,    0, -0.5) // Right
                    << QVector3D(-0.5,    0, -0.5) << QVector3D( 0.5,    0, -0.5) << QVector3D( 0.5,    0,  0.5) // Bottom
                    << QVector3D( 0.5,    0,  0.5) << QVector3D(-0.5,    0,  0.5) << QVector3D(-0.5,    0, -0.5);
  spotlightColours  << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Front
                    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Back
                    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Left
                    << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) << QVector3D(0.2, 0.2, 0.2) // Right
                    << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1) // Bottom
                    << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1) << QVector3D(  1,   1,   1);
}

void GlWidget::resizeGL(int width, int height) {
  if (height == 0) {
    height = 1;
  }

  pMatrix.setToIdentity();
  pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);
  glViewport(0, 0, width, height);
}

void GlWidget::drawLand(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix) {
  QMatrix3x3 normalMatrix;
  normalMatrix = mvMatrix.normalMatrix();

  simpleShaderProgram.bind();
  simpleShaderProgram.setUniformValue("mvpMatrix", pMatrix * viewMatrix);
  simpleShaderProgram.setAttributeArray("vertex", landVertices.constData());
  simpleShaderProgram.enableAttributeArray("vertex");
  simpleShaderProgram.setUniformValue("color", QColor(193, 232, 191));
  simpleShaderProgram.enableAttributeArray("color");

  glDrawArrays(GL_TRIANGLES, 0, landVertices.size());

  simpleShaderProgram.disableAttributeArray("vertex");
  simpleShaderProgram.disableAttributeArray("normal");

  simpleShaderProgram.release();
}

void GlWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 mMatrix;
  QMatrix4x4 viewMatrix;

  QMatrix4x4 cameraTransformation;
  cameraTransformation.rotate(alpha, 0, 1, 0);
  cameraTransformation.rotate(beta, 1, 0, 0);

  QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
  QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

  viewMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

  mMatrix.setToIdentity();

  QMatrix4x4 mvMatrix;
  mvMatrix = viewMatrix * mMatrix;

  QMatrix4x4 lightTransformation;
  lightTransformation.rotate(0.0, 0, 1, 0);

  QVector3D lightPosition = lightTransformation * QVector3D(0, 1, 1);

  mvMatrix.translate(moveLeft, 0, moveUp);

  drawLand(viewMatrix, mvMatrix);
  QMatrix4x4 currentMatrix = mvMatrix;
  foreach(Section* section, roads[0]->leftSections) {
      Building* building = buildingFactory.GetBuilding(section->zone, section->numTentants);

      if(building == NULL)
          continue;

      Model m(&lightingShaderProgram, building->GetHeight(), building->GetColour());
      m.Draw(viewMatrix, currentMatrix, lightPosition, pMatrix);
      currentMatrix.translate(1, 0, 0);
  }

  mMatrix.setToIdentity();
  mMatrix.translate(lightPosition);
  mMatrix.rotate(0.0, 0, 1, 0);
  mMatrix.rotate(45, 1, 0, 0);
  mMatrix.scale(0.1);

  colouringShaderProgram.bind();
  colouringShaderProgram.setUniformValue("mvpMatrix", pMatrix * viewMatrix * mMatrix);
  colouringShaderProgram.setAttributeArray("vertex", spotlightVertices.constData());
  colouringShaderProgram.enableAttributeArray("vertex");
  colouringShaderProgram.setAttributeArray("color", spotlightColours.constData());
  colouringShaderProgram.enableAttributeArray("color");

  glDrawArrays(GL_TRIANGLES, 0, spotlightVertices.size());

  colouringShaderProgram.disableAttributeArray("vertex");
  colouringShaderProgram.disableAttributeArray("color");
  colouringShaderProgram.release();
}

void GlWidget::mousePressEvent(QMouseEvent *event) {
  lastMousePosition = event->pos();
  event->accept();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event) {
  int deltaX = event->x() - lastMousePosition.x();
  int deltaY = event->y() - lastMousePosition.y();

  if(event->buttons() & Qt::MiddleButton) {
    alpha -= deltaX;
    while(alpha < 0)
      alpha += 360;

    while(alpha >= 360)
      alpha -= 360;

    beta -= deltaY;
    if(beta < -90)
      beta = -90;

    if(beta > 90)
      beta = 90;

    updateGL();
  }

  panUp    = (event->y() < 20);
  panDown  = (event->y() > (height() - 20));
  panLeft  = (event->x() < 20);
  panRight = (event->x() > (width() - 20));

  bool shouldPan = (panLeft || panRight || panUp || panDown);

  if(shouldPan && !panTimer->isActive())
      panTimer->start(10);
  else if(!shouldPan && panTimer->isActive())
      panTimer->stop();

  lastMousePosition = event->pos();
  event->accept();
}

void GlWidget::wheelEvent(QWheelEvent *event) {
  int delta = event->delta();

  if(event->orientation() == Qt::Vertical) {
    if(delta < 0)
      distance *= 1.1;
    else if(delta > 0)
      distance *= 0.9;

    updateGL();
  }

  event->accept();
}

void GlWidget::updatePan() {
    double angle = alpha;

    if(panDown)
        angle += 180;
    else if(panLeft)
        angle += 90;
    else if(panRight)
        angle += 270;

    double angleInRads = (angle * M_PI)/180.0;
    moveUp   += (cos(angleInRads) * 0.05);
    moveLeft += (sin(angleInRads) * 0.05);
    updateGL();
}
