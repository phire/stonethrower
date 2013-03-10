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

private:
    QBackingStore *m_backingStore;
    bool m_update_pending;
};
