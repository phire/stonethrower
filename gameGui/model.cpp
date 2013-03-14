#include "model.h"

#include <QStringList>

Model::Model(QGLShaderProgram* shaderProgram, double height, QColor colour) {
    this->shaderProgram = shaderProgram;
    this->colour = colour;

    vertices << QVector3D(-0.4, 0,  0.4) << QVector3D( 0.4, 0,  0.4) << QVector3D( 0.4,  height,  0.4) // Front
             << QVector3D( 0.4, height,  0.4) << QVector3D(-0.4,  height,  0.4) << QVector3D(-0.4, 0,  0.4)
             << QVector3D( 0.4, 0, -0.4) << QVector3D(-0.4, 0, -0.4) << QVector3D(-0.4,  height, -0.4) // Back
             << QVector3D(-0.4, height, -0.4) << QVector3D( 0.4,  height, -0.4) << QVector3D( 0.4, 0, -0.4)
             << QVector3D(-0.4, 0, -0.4) << QVector3D(-0.4, 0,  0.4) << QVector3D(-0.4,  height,  0.4) // Left
             << QVector3D(-0.4, height,  0.4) << QVector3D(-0.4,  height, -0.4) << QVector3D(-0.4, 0, -0.4)
             << QVector3D( 0.4, 0,  0.4) << QVector3D( 0.4, 0, -0.4) << QVector3D( 0.4,  height, -0.4) // Right
             << QVector3D( 0.4, height, -0.4) << QVector3D( 0.4,  height,  0.4) << QVector3D( 0.4, 0,  0.4)
             << QVector3D(-0.4, height,  0.4) << QVector3D( 0.4,  height,  0.4) << QVector3D( 0.4,  height, -0.4) // Top
             << QVector3D( 0.4, height, -0.4) << QVector3D(-0.4, height, -0.4) << QVector3D(-0.4,  height,  0.4)
             << QVector3D(-0.4, 0, -0.4) << QVector3D( 0.4, 0, -0.4) << QVector3D( 0.4, 0,  0.4) // Bottom
             << QVector3D( 0.4, 0,  0.4) << QVector3D(-0.4, 0,  0.4) << QVector3D(-0.4, 0, -0.4);

    normals  << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) << QVector3D( 0,  0,  1) // Front
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
}

QVector<QVector3D> Model::LoadFromCollada() {
    QVector<QVector3D> vertices;
    return vertices;
}

void Model::Draw(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix, QVector3D lightPosition, QMatrix4x4 pMatrix) {
    QMatrix3x3 normalMatrix;
    normalMatrix = mvMatrix.normalMatrix();

    shaderProgram->bind();

    shaderProgram->setUniformValue("mvpMatrix", pMatrix * mvMatrix);
    shaderProgram->setUniformValue("mvMatrix", mvMatrix);
    shaderProgram->setUniformValue("normalMatrix", normalMatrix);
    shaderProgram->setUniformValue("lightPosition", viewMatrix * lightPosition);

    shaderProgram->setUniformValue("ambientColor", QColor(32, 32, 32));
    shaderProgram->setUniformValue("diffuseColor", QColor(128, 128, 128));
    shaderProgram->setUniformValue("specularColor", QColor(255, 255, 255));
    shaderProgram->setUniformValue("ambientReflection", (GLfloat) 1.0);
    shaderProgram->setUniformValue("diffuseReflection", (GLfloat) 1.0);
    shaderProgram->setUniformValue("specularReflection", (GLfloat) 1.0);
    shaderProgram->setUniformValue("shininess", (GLfloat) 100.0);
    shaderProgram->setUniformValue("texture", 0);
    shaderProgram->setUniformValue("colour", colour);

    shaderProgram->setAttributeArray("vertex", vertices.constData());
    shaderProgram->enableAttributeArray("vertex");
    shaderProgram->setAttributeArray("normal", normals.constData());
    shaderProgram->enableAttributeArray("normal");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    shaderProgram->disableAttributeArray("vertex");
    shaderProgram->disableAttributeArray("normal");

    shaderProgram->release();
}

