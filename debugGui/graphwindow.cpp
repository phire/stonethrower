#include "graphwindow.h"

GraphWindow::GraphWindow(QWindow *parent)
  : QWindow(parent), m_update_pending(false), showGuide(false) {
	edgeStart = NULL;
    m_backingStore = new QBackingStore(this);
    create();

    resize(1000, 600);

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
    painter->setPen(roadPen);
    for(int i = 0; i < graph.edges.size(); i++) {
        painter->drawLine(graph.edges.at(i)->toLine());
    }

    if(showGuide)
        painter->drawLine(edgeStart->pos.toPoint(), mouseCurrent);

	const Intersection *snapNode = graph.nodeAt(mouseCurrent);
	if(snapNode != NULL) {
		painter->drawEllipse(snapNode->toPoint(), 15, 15);
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
        edgeStart = graph.nodeAt(e->pos());
		if(!edgeStart)
			edgeStart = new Intersection(e->pos());
        showGuide = true;
    }
}

void GraphWindow::mouseReleaseEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
		Intersection *edgeEnd = graph.nodeAt(e->pos());
		if(!edgeEnd)
			edgeEnd = new Intersection(e->pos());
		graph.addEdge(edgeStart, edgeEnd);
        showGuide = false;
    }
}

void GraphWindow::mouseMoveEvent(QMouseEvent *e) {
    mouseCurrent = e->pos();
	renderLater();
}
