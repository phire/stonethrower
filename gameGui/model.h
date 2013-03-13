#pragma once

#include <QtOpenGL>

class Model
{
public:
    Model();
    QVector<QVector3D> LoadFromCollada();
};
