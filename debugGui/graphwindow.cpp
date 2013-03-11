#include "graphwindow.h"

GraphWindow::GraphWindow(QWindow *parent)
  : QWindow(parent), m_update_pending(false), showGuide(false) {
    m_backingStore = new QBackingStore(this);
    create();

    resize(1000, 600);

	QObject::connect(&graph, SIGNAL(changed()), this, SLOT(RenderLater()));

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
        painter->drawLine(graph.edges.at(i).toLine());
    }

    if(showGuide)
        painter->drawLine(lineStart, mouseCurrent);
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
        mouseCurrent = lineStart = e->pos();
        showGuide = true;
    }
}

void GraphWindow::mouseReleaseEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
		Intersection n1(lineStart);
		Intersection n2(e->pos());
		graph.addNode(n1);
		graph.addNode(n2);
		graph.addEdge(n1, n2);
        showGuide = false;
    }
}

void GraphWindow::mouseMoveEvent(QMouseEvent *e) {
    if(showGuide) {
        mouseCurrent = e->pos();
        renderLater();
    }
}
