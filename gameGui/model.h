#pragma once

#include <QtOpenGL>

class Model {
public:
    Model(QGLShaderProgram* shaderProgram, double height, QColor colour, QString modelFile = "");

    QVector<QVector3D> LoadFromCollada();

    void Draw(QMatrix4x4 viewMatrix, QMatrix4x4 mvMatrix, QVector3D lightPosition, QMatrix4x4 pMatrix);
private:
    QGLShaderProgram* shaderProgram;

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QColor colour;

    void LoadObj(QString path);
    double min(double one, double two);
    double max(double one, double two);
};
