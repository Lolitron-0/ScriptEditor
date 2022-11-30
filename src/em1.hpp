#ifndef EM1_HPP
#define EM1_HPP

#include "block.hpp"
#include <QTimer>
#include <QWidget>

struct Graph {
    Graph() = default;
    Graph(QString line, QPoint pos)
        :line(line), pos(pos)
    {
    }

    QVector<Graph*> children;
    QString line;
    QPoint pos;
};

class Em1 : public QWidget
{
public:
    Em1(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:

    void renderGraph(QPainterPath *, Graph*);

    QImage mGrid;
    Block block;
    Graph* mGraph;
    Graph mTempNode;
    Graph* mCurrentNode;

    QTimer mRefreshTimer;

};

#endif // EM1_HPP
