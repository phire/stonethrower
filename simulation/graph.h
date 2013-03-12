#include <QtGui>

class Road;
class Intersection;

class Graph : public QObject {
	Q_OBJECT
public:
	//explicit Graph()

	void addNode(Intersection *node);
	void addEdge(Intersection *n1, Intersection *n2);

	Intersection* nodeAt(QPoint);
	Road* roadAt(QPoint);

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
	Intersection(QPoint);
	Intersection(QVector2D);

	void addEdge(Road*);
	void delEdge(Road*);
	QPoint toPoint() const;
	Road *edgeTo(Intersection other);

public:
	QSet<Road*> edges;
	QVector2D pos; // note to self, check QPoint generates optimal code
};

class Road { // aka edge
public:
	Road(Intersection *n1, Intersection *n2);
	
	QLine toLine() const;
	bool intersect(Road *other, QVector2D *at);


	Intersection *start;
	Intersection *end;
	float length;
};


