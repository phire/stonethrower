#include "graphwindow.h"

GraphWindow::GraphWindow(QWindow *parent)
  : QWindow(parent), m_update_pending(false), showGuide(false) {
	edgeStart = NULL;
    m_backingStore = new QBackingStore(this);
    create();

    resize(1000, 600);
	scale = 1000;

	QObject::connect(&graph, SIGNAL(changed()), this, SLOT(renderLater()));
}

void GraphWindow::exposeEvent(QExposeEvent *) {
    if(isExposed()) 
        renderNow();
}

void GraphWindow::resizeEvent(QResizeEvent *resizeEvent) {
    m_backingStore->resize(resizeEvent->size());
    if (isExposed())
        renderNow();
}

void GraphWindow::renderNow() {
    if (!isExposed())
        return;

    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    painter.fillRect(0, 0, width(), height(), Qt::white);
    render(&painter);

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

void GraphWindow::render(QPainter *painter) {
    static QPen roadPen(Qt::SolidPattern, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	static QPen sectionPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    for(int i = 0; i < graph.edges.size(); i++) {
		painter->setPen(roadPen);
		Road *road = graph.edges.at(i);
		auto coords = road->getCoords();
        painter->drawLine(((*coords[0]) * scale).toPoint(), 
						  ((*coords[1]) * scale).toPoint());

		painter->setPen(sectionPen);
		for(int j = 0; j < road->leftSections.size(); j++) {
			Section *l = road->leftSections.at(j);
			if(!l) continue;
			QPointF pointsl[5] = {(l->coords[0] * scale).toPoint(),
							 	 (l->coords[1] * scale).toPoint(),
							 	 (l->coords[2] * scale).toPoint(),
							 	 (l->coords[3] * scale).toPoint(),
								 (l->coords[0] * scale).toPoint()};
			painter->drawPolyline(pointsl, 5);

			Section *r = road->rightSections.at(j);
			QPointF pointsr[5] = {(r->coords[0] * scale).toPoint(),
							 	 (r->coords[1] * scale).toPoint(),
							 	 (r->coords[2] * scale).toPoint(),
							 	 (r->coords[3] * scale).toPoint(),
								 (r->coords[0] * scale).toPoint()};
			painter->drawPolyline(pointsr, 5);
		} 
    }
	painter->setPen(roadPen);

    if(showGuide)
        painter->drawLine((edgeStart->pos*scale).toPoint(), mouseCurrent);

	Intersection *snapNode = graph.nodeAt(QVector2D(mouseCurrent)/scale);
	if(snapNode != NULL) {
		painter->drawEllipse((snapNode->pos*scale).toPoint(), 15, 15);
	}
}

void GraphWindow::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool GraphWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        m_update_pending = false;
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

void GraphWindow::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
		auto v = QVector2D(e->pos()) / scale;
        edgeStart = graph.nodeAt(v);
		if(!edgeStart)
			edgeStart = new Intersection(v);
        showGuide = true;
    }
}

void GraphWindow::mouseReleaseEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
		auto v = QVector2D(e->pos()) / scale;
		Intersection *edgeEnd = graph.nodeAt(v);
		if(!edgeEnd)
			edgeEnd = new Intersection(v);
		graph.addEdge(edgeStart, edgeEnd);
        showGuide = false;
    }
}

void GraphWindow::mouseMoveEvent(QMouseEvent *e) {
    mouseCurrent = e->pos();
	renderLater();
}
