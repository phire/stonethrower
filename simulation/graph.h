#pragma once

#include <QtGui>
#include <array>

class Road;
class Intersection;
class Section;

class Graph : public QObject {
	Q_OBJECT
public:
	//explicit Graph()

	void addNode(Intersection *node);
	void addEdge(Intersection *n1, Intersection *n2);

	Intersection* nodeAt(QVector2D);
	Road* roadAt(QVector2D);
    Section* sectionAt(QVector2D);

private:
	void splitEdges(Road *newEdge);
	Road* split(Road *edge1, Road *edge2, const QVector2D at);

signals:
	void changed();

public:
	QSet<Intersection*> nodes;
	QList<Road*> edges;
};

class Intersection { // aka node
public:
	Intersection(QVector2D);

	void addEdge(Road*);
	void delEdge(Road*);
	QPoint toPoint() const;
	Road *edgeTo(Intersection other);

public:
	QSet<Road*> edges;
	QVector2D pos; 
};

class Road { // aka edge
public:
	Road(Intersection *n1, Intersection *n2);
	
	std::array<QVector2D*, 2> getCoords() const;
	bool intersect(Road *other, QVector2D *at);
    float distance(QVector2D);
	
	Intersection *start;
	Intersection *end;
	float length;
    QVector<Section*> leftSections;
    QVector<Section*> rightSections;
};

class Section {
public:
	enum zone {Unzoned, Residential, Commercial, Industrial};
	Section(): zone(Unzoned) {};
    Section(Road*, QVector2D, QVector2D, QVector2D, QVector2D);

	bool containsPoint(QVector2D) const;

public:
	int zone;
    int numTentants;
    Road *road;
	QVector2D coords[4];
};
