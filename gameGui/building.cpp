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

    printf("Building count: %d, type: %d\n", buildings.count(), type);
    fflush(stdout);

    if(buildings.count() <= type || type == 0)
        return NULL;

    foreach(Building* b, buildings[type]) {
        if(numTenants < b->GetMaxTenants())
            return b;
    }

    return NULL;
}

void BuildingFactory::InitBuildings() {
    buildings << QVector<Building*>(); // unzoned

    buildings << QVector<Building*>();
    buildings[Section::Residential] << new Building(0.5, 5, Qt::green);
    buildings[Section::Residential] << new Building(1, 20, Qt::green);
    buildings[Section::Residential] << new Building(3, 300, Qt::darkGreen);

    buildings << QVector<Building*>();
    buildings[Section::Commercial] << new Building(0.5, 5, Qt::blue);
    buildings[Section::Commercial] << new Building(1, 20, Qt::blue);
    buildings[Section::Commercial] << new Building(3, 300, Qt::blue);

    buildings << QVector<Building*>();
    buildings[Section::Industrial] << new Building(0.5, 5, Qt::yellow);
    buildings[Section::Industrial] << new Building(1, 20, Qt::yellow);
    buildings[Section::Industrial] << new Building(3, 300, Qt::darkYellow);
}
