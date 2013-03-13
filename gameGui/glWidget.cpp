#include "glWidget.h"
#include "model.h"

GlWidget::GlWidget(QWidget *parent)
  : QGLWidget(QGLFormat(/* Additional format options */), parent)
{
  alpha = 25;
  beta= -25;
  distance = 2.5;
}

GlWidget::~GlWidget() {
}

QSize GlWidget::sizeHint() const {
  return QSize(640, 480);
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

  cubeVertices << QVector3D(-0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) // Front
           << QVector3D( 0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5)
           << QVector3D( 0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) // Back
           << QVector3D(-0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5)
           << QVector3D(-0.5, -0.5, -0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5,  0.5,  0.5) // Left
           << QVector3D(-0.5,  0.5,  0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5, -0.5, -0.5)
           << QVector3D( 0.5, -0.5,  0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5,  0.5, -0.5) // Right
           << QVector3D( 0.5,  0.5, -0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5, -0.5,  0.5)
           << QVector3D(-0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5,  0.5) << QVector3D( 0.5,  0.5, -0.5) // Top
           << QVector3D( 0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5, -0.5) << QVector3D(-0.5,  0.5,  0.5)
           << QVector3D(-0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5, -0.5) << QVector3D( 0.5, -0.5,  0.5) // Bottom
           << QVector3D( 0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5,  0.5) << QVector3D(-0.5, -0.5, -0.5);

  cubeNormals << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) // Front
              << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1)
              << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1) // Back
              << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1) << QVector3D( 0,  0, -1)
              << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0) // Left
              << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0) << QVector3D(-1,  0,  0)
              << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0) // Right
              << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0) << QVector3D( 1,  0,  0)
              << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0) // Top
              << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0) << QVector3D( 0,  1,  0)
              << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0) // Bottom
              << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0) << QVector3D( 0, -1,  0);

  cubeColours << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) // Front
          << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) << QVector3D(1, 0, 0)
          << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) // Back
          << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) << QVector3D(1, 0, 0)
          << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) // Left
          << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0)
          << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) // Right
          << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0)
          << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) // Top
          << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) << QVector3D(0, 0, 1)
          << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) // Bottom
          << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) << QVector3D(0, 0, 1);

  landVertices << QVector3D(-10, -0.5, -10) << QVector3D(-10, -0.5, 10) << QVector3D(10, -0.5, 10)
               << QVector3D(10, -0.5, 10) <<  QVector3D(10, -0.5, -10) << QVector3D(-10, -0.5, -10);

  landColours << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0)
              << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0);


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

void GlWidget::drawLand(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix, QVector3D lightPosition) {
  QMatrix3x3 normalMatrix;
  normalMatrix = mvMatrix.normalMatrix();

  lightingShaderProgram.bind();

  lightingShaderProgram.setUniformValue("mvpMatrix", pMatrix * mvMatrix);
  lightingShaderProgram.setUniformValue("mvMatrix", mvMatrix);
  lightingShaderProgram.setUniformValue("normalMatrix", normalMatrix);
  lightingShaderProgram.setUniformValue("lightPosition", viewMatrix * lightPosition);

  lightingShaderProgram.setUniformValue("ambientColor", QColor(32, 32, 32));
  lightingShaderProgram.setUniformValue("diffuseColor", QColor(128, 128, 128));
  lightingShaderProgram.setUniformValue("specularColor", QColor(255, 255, 255));
  lightingShaderProgram.setUniformValue("ambientReflection", (GLfloat) 1.0);
  lightingShaderProgram.setUniformValue("diffuseReflection", (GLfloat) 1.0);
  lightingShaderProgram.setUniformValue("specularReflection", (GLfloat) 1.0);
  lightingShaderProgram.setUniformValue("shininess", (GLfloat) 100.0);
  lightingShaderProgram.setUniformValue("texture", 0);

  lightingShaderProgram.setAttributeArray("vertex", landVertices.constData());
  lightingShaderProgram.enableAttributeArray("vertex");
  lightingShaderProgram.setAttributeArray("normal", cubeNormals.constData());
  lightingShaderProgram.enableAttributeArray("normal");
  lightingShaderProgram.setAttributeArray("colour", landColours.constData());
  lightingShaderProgram.enableAttributeArray("colour");

  glDrawArrays(GL_TRIANGLES, 0, landVertices.size());

  lightingShaderProgram.disableAttributeArray("vertex");
  lightingShaderProgram.disableAttributeArray("normal");

  lightingShaderProgram.release();
}

void GlWidget::drawModel(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix, QVector3D lightPosition) {
    QMatrix3x3 normalMatrix;
    normalMatrix = mvMatrix.normalMatrix();

    Model m;
    QVector<QVector3D> vertices = m.LoadFromCollada();

    simpleShaderProgram.bind();

    simpleShaderProgram.setUniformValue("mvpMatrix", pMatrix * mvMatrix);
    simpleShaderProgram.setUniformValue("mvMatrix", mvMatrix);
    simpleShaderProgram.setUniformValue("normalMatrix", normalMatrix);
    simpleShaderProgram.setUniformValue("lightPosition", viewMatrix * lightPosition);

    simpleShaderProgram.setUniformValue("color", QColor(Qt::darkGreen));

    simpleShaderProgram.setAttributeArray("vertex", vertices.constData());
    simpleShaderProgram.enableAttributeArray("vertex");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    simpleShaderProgram.disableAttributeArray("vertex");

    simpleShaderProgram.release();
}

void GlWidget::drawCube(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix, QVector3D lightPosition) {
  QMatrix3x3 normalMatrix;
  normalMatrix = mvMatrix.normalMatrix();

  lightingShaderProgram.bind();

  lightingShaderProgram.setUniformValue("mvpMatrix", pMatrix * mvMatrix);
  lightingShaderProgram.setUniformValue("mvMatrix", mvMatrix);
  lightingShaderProgram.setUniformValue("normalMatrix", normalMatrix);
  lightingShaderProgram.setUniformValue("lightPosition", viewMatrix * lightPosition);

  lightingShaderProgram.setUniformValue("ambientColor", QColor(32, 32, 32));
  lightingShaderProgram.setUniformValue("diffuseColor", QColor(128, 128, 128));
  lightingShaderProgram.setUniformValue("specularColor", QColor(255, 255, 255));
  lightingShaderProgram.setUniformValue("ambientReflection", (GLfloat) 1.0);
  lightingShaderProgram.setUniformValue("diffuseReflection", (GLfloat) 1.0);
  lightingShaderProgram.setUniformValue("specularReflection", (GLfloat) 1.0);
  lightingShaderProgram.setUniformValue("shininess", (GLfloat) 100.0);
  lightingShaderProgram.setUniformValue("texture", 0);

  lightingShaderProgram.setAttributeArray("vertex", cubeVertices.constData());
  lightingShaderProgram.enableAttributeArray("vertex");
  lightingShaderProgram.setAttributeArray("normal", cubeNormals.constData());
  lightingShaderProgram.enableAttributeArray("normal");
  lightingShaderProgram.setAttributeArray("colour", cubeColours.constData());
  lightingShaderProgram.enableAttributeArray("colour");

  glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

  lightingShaderProgram.disableAttributeArray("vertex");
  lightingShaderProgram.disableAttributeArray("normal");

  lightingShaderProgram.release();
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

  // THE MIDDLE PARAMETER CAN PROBABLY USED TO PAN THE CAMERA
  viewMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

  mMatrix.setToIdentity();

  QMatrix4x4 mvMatrix;
  mvMatrix = viewMatrix * mMatrix;

  QMatrix4x4 lightTransformation;
  lightTransformation.rotate(0.0, 0, 1, 0);

  QVector3D lightPosition = lightTransformation * QVector3D(0, 1, 1);

  drawModel(viewMatrix, mvMatrix, lightPosition);

  drawLand(viewMatrix, mvMatrix, lightPosition);
  drawCube(viewMatrix, mvMatrix, lightPosition);
  QMatrix4x4 newmv = mvMatrix;
  newmv.translate(2, 0, 0);
  drawCube(viewMatrix, newmv, lightPosition);

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
