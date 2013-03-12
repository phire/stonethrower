#include "graph.h"

void Graph::addNode(Intersection *node) {
	nodes << node;
	emit changed();
}

void Graph::addEdge(Intersection *n1, Intersection *n2) {
	// Check edge doesn't already exsist
	QSetIterator<Road *> i(n1->edges);
	while(i.hasNext()) {
		Road *edge = i.next();
		if(edge->start == n2 or edge->end == n2)
			return;
	}

	Road *edge = new Road(n1, n2);
	edges << edge;

	// Make sure nodes are in the collection.
	nodes.insert(n1);
	nodes.insert(n2);

	splitEdges(edge);

	emit changed();
}

void Graph::splitEdges(Road *newEdge) {
	QMap<float, QPair<Road *, QVector2D> > intersectingRoads;
	QVector2D at;

	// Find intersecting nodes and sort them by distance to the first node
	for(int i = 0; i < edges.size(); i++) {
		Road *edge = edges.at(i);
		if(edge == newEdge) continue;
		if(edge->intersect(newEdge, &at)) {
			float distance = (newEdge->start->pos - at).length();
			intersectingRoads[distance] = QPair<Road *, QVector2D>(edge, at);
		}
	}
	
	Road *currentEdge = newEdge;
	QMapIterator<float, QPair<Road *, QVector2D> > i(intersectingRoads);
	while (i.hasNext()) {
		i.next();
		currentEdge = split(currentEdge, i.value().first, i.value().second);
	}
	
}

Road* Graph::split(Road *edge1, Road *edge2, const QVector2D at) {
	Intersection *n = nodeAt(at.toPoint()); 
	if(!n)
		n = new Intersection(at);
	edge2->end->delEdge(edge2);
	Road *edge3 = new Road(n, edge2->end);
	edge2->end = n;
	n->addEdge(edge2);

	edge1->end->delEdge(edge1);
	Road *edge4 = new Road(n, edge1->end);
	edge1->end = n;
	n->addEdge(edge1);

	edges << edge3 << edge4;
	nodes << n;

	return edge4;
}

Intersection* Graph::nodeAt(QPoint p) {
	QVector2D v(p);

	QSetIterator<Intersection *> i(nodes);
	while(i.hasNext()) {
		Intersection *node = i.next();
		if((v - node->pos).length() < 15.0)
			return node;
	}
	return NULL;
}

Road* Graph::roadAt(QPoint) {
	return NULL;
}

Intersection::Intersection(QPoint p) 	: pos(p) {}
Intersection::Intersection(QVector2D p) : pos(p) {}

void Intersection::addEdge(Road *edge) {
	edges << edge;
}

void Intersection::delEdge(Road *edge) {
	edges.remove(edge);
}


QPoint Intersection::toPoint() const {
	return pos.toPoint();
}

Road::Road(Intersection *n1, Intersection *n2) : start(n1), end(n2) {
	length = (n1->pos - n2->pos).length();
	n1->addEdge(this);
	n2->addEdge(this);
}

QLine Road::toLine() const {
	return QLine(start->toPoint(), end->toPoint());
}

bool Road::intersect(Road *other, QVector2D *at) {
	// http://paulbourke.net/geometry/pointlineplane/
	float x1 = start->pos.x(), y1 = start->pos.y();
	float x2 = end->pos.x(), y2 = end->pos.y();
	float x3 = other->start->pos.x(), y3 = other->start->pos.y();
	float x4 = other->end->pos.x(), y4 = other->end->pos.y();


	float denom  = (y4-y3) * (x2-x1) - (x4-x3) * (y2-y1);
	float numera = (x4-x3) * (y1-y3) - (y4-y3) * (x1-x3);
	float numerb = (x2-x1) * (y1-y3) - (y2-y1) * (x1-x3);

	// Are lines coincident? (we can possibly skip this check)
	if(fabs(numera) < 0.000001 && fabs(numerb) < 0.000001 && fabs(denom) < 0.000001) {
		*at = QVector2D((x1+x2)/2, (y1+y2)/2);
		return true;
	}

	// Are lines parallel?
	if(fabs(denom) < 0.000001) 
		return false;

	// Is the intersection along the segments?
	float mua = numera / denom;
	float mub = numerb / denom;
	if (mua < 0 || mua > 1 || mub < 0 || mub > 1)
		return false;

	*at = QVector2D(x1 + mua * (x2 - x1), y1 + mua * (y2 - y1));
	return true;
}

