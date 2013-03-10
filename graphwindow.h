#include <QtGui>

class GraphWindow : public QWindow {
    Q_OBJECT
public:
    explicit GraphWindow(QWindow *parent = 0);
    virtual void render(QPainter *painter);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event);

    void resizeEvent(QResizeEvent *event);
    void exposeEvent(QExposeEvent *event);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);



private:
    QBackingStore *m_backingStore;
    bool m_update_pending;

    QPoint lineStart;
    QPoint mouseCurrent;
    bool showGuide;
    QVector<QLine> lines;
};
