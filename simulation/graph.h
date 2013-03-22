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

    float *pathTable = new float[0];
};

QDataStream &operator<<(QDataStream &, const Graph &);
QDataStream &operator>>(QDataStream &, Graph &);

class Intersection { // aka node
public:
	Intersection(QVector2D);
    Intersection(int num, QVector2D pos);

	void addEdge(Road*);
	void delEdge(Road*);
	QPoint toPoint() const;
	Road *edgeTo(Intersection other);

public:
	QSet<Road*> edges;
    QVector2D pos;
    int num;
    static int count;
};

QDataStream &operator<<(QDataStream &, const Intersection &);
QDataStream &operator>>(QDataStream &, Intersection *&);

class Road { // aka edge
public:
	Road(Intersection *n1, Intersection *n2);
    Road(int Start, int end, QVector<Section*> left, QVector<Section*> right);

    void fixup(QSet<Intersection *> nodes);
	
	std::array<QVector2D*, 2> getCoords() const;
	bool intersect(Road *other, QVector2D *at);
    float distance(QVector2D);
	
	Intersection *start;
	Intersection *end;
	float length;
    QVector<Section*> leftSections;
    QVector<Section*> rightSections;
};

QDataStream &operator<<(QDataStream &, const Road &);
QDataStream &operator>>(QDataStream &, Road *&);

class Section {
public:
    enum zone {Unzoned, Residential, Commercial, Industrial};
    Section(): zone(Unzoned) {}
    Section(Road*, QVector2D, QVector2D, QVector2D, QVector2D);
    Section(int zone, int numTentants, QVector2D a, QVector2D b, QVector2D c, QVector2D d);

	bool containsPoint(QVector2D) const;

public:
	int zone;
    int numTentants;
    Road *road;
	QVector2D coords[4];
};

QDataStream &operator<<(QDataStream &, const Section &);
QDataStream &operator>>(QDataStream &, Section *&);
