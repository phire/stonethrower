#include "graphwindow.h"

GraphWindow::GraphWindow(QWindow *parent)
  : QWindow(parent), m_update_pending(false), showGuide(false) {
	edgeStart = NULL;
    m_backingStore = new QBackingStore(this);
    create();

    resize(1000, 600);
	scale = 1000;
    mode = 1;

    QFile file("save.sto");
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QByteArray ba = qUncompress(file.readAll());
        QDataStream in(ba);
        in >> graph;
    }

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

void GraphWindow::drawSection(QPainter *painter, const Section *s) {
    QPointF points[5] = {(s->coords[0] * scale).toPoint(),
                         (s->coords[1] * scale).toPoint(),
                         (s->coords[2] * scale).toPoint(),
                         (s->coords[3] * scale).toPoint(),
                         (s->coords[0] * scale).toPoint()};

    static QPen highlightPen(Qt::SolidPattern, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    if(s->containsPoint(QVector2D(mouseCurrent)/scale)) {
        painter->setPen(highlightPen);
        painter->drawPolyline(points, 5);
    }


	static QPen unzonedPen(Qt::gray, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    static QPen residentialPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    static QPen commercialPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    static QPen industrialPen(Qt::yellow, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    switch(s->zone) {
    case Section::Unzoned:
        return; // return
        painter->setPen(unzonedPen);
        break;
    case Section::Residential:
        painter->setPen(residentialPen);
        break;
    case Section::Commercial:
        painter->setPen(commercialPen);
        break;
    case Section::Industrial:
        painter->setPen(industrialPen);
        break;
    }

	painter->drawPolyline(points, 5);

}

void GraphWindow::render(QPainter *painter) {
    static QPen roadPen(Qt::SolidPattern, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    for(int i = 0; i < graph.edges.size(); i++) {
		painter->setPen(roadPen);
		Road *road = graph.edges.at(i);
		auto coords = road->getCoords();
        painter->drawLine(((*coords[0]) * scale).toPoint(), 
						  ((*coords[1]) * scale).toPoint());

		for(int j = 0; j < road->leftSections.size(); j++) {
			drawSection(painter, road->leftSections.at(j));
			drawSection(painter, road->rightSections.at(j));
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
    switch(mode) {
    case 1:
        if(e->button() == Qt::LeftButton) {
            auto v = QVector2D(e->pos()) / scale;
            edgeStart = graph.nodeAt(v);
            if(!edgeStart)
                edgeStart = new Intersection(v);
            showGuide = true;
        }
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        if(e->button() == Qt::LeftButton) {
            auto v = QVector2D(e->pos()) / scale;
            auto s = graph.sectionAt(v);
            if(!s)
                return;
            s->zone = (mode - 2) + Section::Unzoned;
        }
        break;
    }
}

void GraphWindow::mouseReleaseEvent(QMouseEvent *e) {
    switch(mode) {
    case 1:
        if(e->button() == Qt::LeftButton) {
            showGuide = false;
            auto v = QVector2D(e->pos()) / scale;
            if((v - edgeStart->pos).length() < 0.024) {
                if (edgeStart->edges.size() == 0)
                    delete edgeStart;
                return;
            }
            Intersection *edgeEnd = graph.nodeAt(v);
            if(!edgeEnd)
                edgeEnd = new Intersection(v);
            graph.addEdge(edgeStart, edgeEnd);
        }
        break;
    }
}

void GraphWindow::mouseMoveEvent(QMouseEvent *e) {
    mouseCurrent = e->pos();
	renderLater();
}

void GraphWindow::keyReleaseEvent(QKeyEvent *e) {
    switch(e->key()) {
    case Qt::Key_1:
        mode = 1;
        break;
    case Qt::Key_2:
        mode = 2;
        break;
    case Qt::Key_3:
        mode = 3;
        break;
    case Qt::Key_4:
        mode = 4;
        break;
    case Qt::Key_5:
        mode = 5;
        break;
    case Qt::Key_S:
        qDebug("Saving");
        QFile file("save.sto");
        file.open(QIODevice::WriteOnly);
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QDataStream out(&buffer);
        out << graph;
        file.write(qCompress(buffer.buffer()));
        break;
    }
}
