#include "savefile.h"

SaveFile::SaveFile(QString filename) {
    this->filename = filename;
}

QList<Road*> SaveFile::GetRoads() {
    QFile file(filename);
    file.open(QFile::ReadOnly);
    printf("Filename: %s\n", qPrintable(filename));

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());

    QList<Road*> roads;

    qDebug("Found %d objects\n", document.object()["roads"].toArray().count());

    foreach(QJsonValue road, document.object()["roads"].toArray()) {
        roads << LoadRoad(road.toObject());
    }

    return roads;
}

Intersection* SaveFile::FindIntersection(QVector2D coords) {
    foreach(Intersection* i, intersections) {
        if(i->pos == coords)
            return i;
    }

    return NULL;
}

Road* SaveFile::LoadRoad(QJsonObject jsonRoad) {
    QJsonArray intersections = jsonRoad["coords"].toArray();

    qDebug("Json array size for coords: %d\n", intersections.count());

    Intersection* start = LoadIntersection(intersections.at(0).toObject());
    Intersection* end   = LoadIntersection(intersections.at(1).toObject());

    Road* road = new Road(start, end);

    foreach(QJsonValue section, jsonRoad["left-sections"].toArray())
        road->leftSections.append(LoadSection(section.toObject()));

    foreach(QJsonValue section, jsonRoad["right-sections"].toArray())
        road->rightSections.append(LoadSection(section.toObject()));

    return road;
}

Section* SaveFile::LoadSection(QJsonObject jsonSection) {
    Section* section = new Section();
    section->zone = (int)jsonSection["type"].toDouble();
    section->numTentants = (int)jsonSection["numTenants"].toDouble();

    return section;
}

Intersection* SaveFile::LoadIntersection(QJsonObject jsonIntersection) {
    double x = jsonIntersection["x"].toDouble();
    double y = jsonIntersection["y"].toDouble();

    QVector2D coords(x, y);
    Intersection* i = FindIntersection(coords);
    if(i)
        return i;

    Intersection* intersection = new Intersection(coords);
    intersections << intersection;
    return intersection;
}
