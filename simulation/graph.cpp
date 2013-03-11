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
		if(edge->node1 == n2 or edge->node2 == n2)
			return;
	}

	Road *edge = new Road(n1, n2);
	edges << edge;
	nodes.insert(n1);
	nodes.insert(n2);
	emit changed();
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

Intersection::Intersection(QPoint p) : pos(p) {}

void Intersection::addEdge(Road *edge) {
	edges << edge;
}

QPoint Intersection::toPoint() const {
	return pos.toPoint();
}

Road::Road(Intersection *n1, Intersection *n2) : node1(n1), node2(n2) {
	length = (n1->pos - n2->pos).length();
	n1->addEdge(this);
	n2->addEdge(this);
}

QLine Road::toLine() const {
	return QLine(node1->toPoint(), node2->toPoint());
}
