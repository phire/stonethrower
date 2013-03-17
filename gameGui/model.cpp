#include "model.h"

#include <QStringList>

Model::Model(QGLShaderProgram* shaderProgram, double height, QColor colour, QString modelPath) {
    this->shaderProgram = shaderProgram;
    this->colour = colour;

    if(modelPath == "") {
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
    else {
        LoadObj(modelPath);
    }
}

double Model::max(double one, double two) {
    return one > two ? one : two;
}

double Model::min(double one, double two) {
    return one > two ? two : one;
}

void Model::LoadObj(QString modelPath) {
    QFile modelFile(modelPath);
    modelFile.open(QFile::ReadOnly);

    QVector<QVector3D> coords, normalCoords;

    // do a pre-parse to find a number to scale by and how much to move up on the y axis
    double largestX, largestY, largestZ, lowestY;
    while(!modelFile.atEnd()) {
        QString line = QString(modelFile.readLine());
        QStringList lineComponents = line.split(" ", QString::SkipEmptyParts);
        if(lineComponents.count() != 4 || lineComponents[0] != "v")
            continue;

        double x = abs(lineComponents[1].toDouble());
        double y = abs(lineComponents[2].toDouble());
        double z = abs(lineComponents[3].toDouble());

        largestX = max(x, largestX);
        largestY = max(y, largestY);
        largestZ = max(z, largestZ);

        double actualY = lineComponents[2].toDouble();
        lowestY = min(actualY, lowestY);
    }

    double yOffset = 0 - lowestY;
    double scale = max(largestX / 0.4, max(largestY / 0.4, largestZ / 0.4));

    modelFile.seek(0);

    while(!modelFile.atEnd()) {
        QString line = QString(modelFile.readLine());
        if(line == "" || line[0] == '#')
            continue;

        QStringList lineComponents = line.split(" ", QString::SkipEmptyParts);
        if(lineComponents.count() != 4)
            continue;

        if(lineComponents[0] == "v") {
            double x = lineComponents[1].toDouble() / scale;
            double y = (lineComponents[2].toDouble() + yOffset)/scale;
            double z = lineComponents[3].toDouble() / scale;

            coords << QVector3D(x, y, z);
        }
        else if(lineComponents[0] == "f") {
            int x = lineComponents[1].toInt() - 1;
            int y = lineComponents[2].toInt() - 1;
            int z = lineComponents[3].toInt() - 1;

            vertices << coords[x] << coords[y] << coords[z];
            normals  << normalCoords[x] << normalCoords[y] << normalCoords[z];
        }
        else if(lineComponents[0] == "vn") {
            double x = lineComponents[1].toDouble();
            double y = lineComponents[2].toDouble();
            double z = lineComponents[3].toDouble();

            normalCoords << QVector3D(x, y, z);
        }

    }

    modelFile.close();
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

