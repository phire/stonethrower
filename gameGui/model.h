#pragma once

#include <QtOpenGL>

class Model {
public:
    Model(QGLShaderProgram* shaderProgram);

    QVector<QVector3D> LoadFromCollada();

    void Draw(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix, QVector3D lightPosition, QMatrix4x4 pMatrix);
private:
    QGLShaderProgram* shaderProgram;

    QVector<QVector3D> vertices;
    QVector<QVector3D> colours;
    QVector<QVector3D> normals;
};
