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

    typedef std::shared_ptr<Block> BlockPtr;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:

    void _addBlock(QPoint pos);
    void _examineUnder(QPointF pos);
    BlockPtr _getHoveredBlock();

    Block* _getWithPendingConnection();

    QImage mGrid;
    QVector<std::shared_ptr<Block>> mBlocks;
    Graph* mGraph;
    Graph mTempNode;
    Graph* mCurrentNode;
    //private add block method to set up connects
    QTimer mRefreshTimer;

};

#endif // EM1_HPP
