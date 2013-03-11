#include <QtGui>

class Road;
class Intersection;

class Graph : public QObject {
	Q_OBJECT
public:
	//explicit Graph()

	void addNode(Intersection node);
	void addEdge(Intersection n1, Intersection n2);

	Intersection* nodeAt(QPoint);
	Road* roadAt(QPoint);

signals:
	void changed();	

public:
	QList<Intersection> nodes;
	QList<Road> edges;
};

class Intersection { // aka node
public:
	Intersection(QPoint);

	void addEdge(Road);
	QPoint toPoint() const;

private:
	QList<Road> edges;
public:
	QPointF pos; // note to self, check QPoint generates optimal code
};

class Road { // aka edge
public:
	Road(Intersection n1, Intersection n2);
	
	QLine toLine() const;

private:
	Intersection node1;
	Intersection node2;
	float length;
};


