#include "building.h"
#include "graph.h"

Building::Building(double height, int maxTenants, QColor colour) {
    this->height = height;
    this->colour = colour;
    this->maxTenants = maxTenants;
}

double Building::GetHeight() {
    return height;
}

QColor Building::GetColour() {
    return colour;
}

int Building::GetMaxTenants() {
    return maxTenants;
}

Building* BuildingFactory::GetBuilding(int type, int numTenants) {
    if(buildings.count() == 0)
        InitBuildings();

    if(buildings.count() <= type || type == 0)
        return NULL;

    foreach(Building* b, buildings[type]) {
        if(numTenants < b->GetMaxTenants() || b->GetMaxTenants() == 0)
            return b;
    }

    return NULL;
}

void BuildingFactory::InitBuildings() {
    buildings << QVector<Building*>(); // unzoned

    buildings << QVector<Building*>();
    buildings[Section::Residential] << new Building(0.5, 5, Qt::green);
    buildings[Section::Residential] << new Building(1, 20, Qt::green);
    buildings[Section::Residential] << new Building(3, 300, Qt::green);

    buildings << QVector<Building*>();
    buildings[Section::Commercial] << new Building(0.5, 5, Qt::blue);
    buildings[Section::Commercial] << new Building(1, 20, Qt::blue);
    buildings[Section::Commercial] << new Building(3, 300, Qt::blue);

    buildings << QVector<Building*>();
    buildings[Section::Industrial] << new Building(0.5, 5, Qt::yellow);
    buildings[Section::Industrial] << new Building(1, 20, Qt::yellow);
    buildings[Section::Industrial] << new Building(3, 300, Qt::yellow);

    buildings << QVector<Building*>();
    buildings[Section::Nuclear] << new Building(1, 0, Qt::red);

    buildings << QVector<Building*>();
    buildings[Section::Coal] << new Building(1, 0, Qt::red);

    buildings << QVector<Building*>();
    buildings[Section::Police] << new Building(1, 0, Qt::red);

    buildings << QVector<Building*>();
    buildings[Section::Fire] << new Building(1, 0, Qt::red);

    buildings << QVector<Building*>();
    buildings[Section::Stadium] << new Building(1, 0, Qt::darkGreen);

    buildings << QVector<Building*>();
    buildings[Section::Park] << new Building(1, 0, Qt::darkGreen);
}
