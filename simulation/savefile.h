#pragma once

// for now this is a JSON file

#include <Qt>
#include "graph.h"

class SaveFile
{
public:
    SaveFile(QString filename);

    QList<Road*> GetRoads();

private:
    QList<Intersection*> intersections;

    Intersection* FindIntersection(QVector2D coords);
    Road* LoadRoad(QJsonObject jsonRoad);
    Intersection* LoadIntersection(QJsonObject jsonIntersection);
    Section* LoadSection(QJsonObject jsonSection);

    QString filename;
};
