#include "graph.h"

void Graph::addNode(Intersection node) {
	nodes << node;
	emit changed();
}

void Graph::addEdge(Intersection n1, Intersection n2) {
	Road edge(n1, n2);
	n1.addEdge(edge);
	n2.addEdge(edge);
	edges << edge;
	emit changed();
}

Intersection* Graph::nodeAt(QPoint) {
	return NULL;
}

Road* Graph::roadAt(QPoint) {
	return NULL;
}

Intersection::Intersection(QPoint p) : pos(p) {}

void Intersection::addEdge(Road edge) {
	edges << edge;
}

QPoint Intersection::toPoint() const {
	return pos.toPoint();
}

Road::Road(Intersection n1, Intersection n2) : node1(n1), node2(n2) {
	length = QLineF(n1.pos, n2.pos).length();
}

QLine Road::toLine() const {
	return QLine(node1.toPoint(), node2.toPoint());
}
