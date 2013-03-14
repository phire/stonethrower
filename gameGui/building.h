#pragma once

#include <Qt>
#include "graph.h"

class Building {
public:
    Building(double height, int maxTenants, QColor colour);

    double GetHeight();
    QColor GetColour();
    int    GetMaxTenants();

private:
    double height;
    QColor colour;
    int maxTenants;
};

class BuildingFactory {
public:
    Building* GetBuilding(int type, int numTenants);

private:
    QVector< QVector<Building*> > buildings;
    void InitBuildings();
};
