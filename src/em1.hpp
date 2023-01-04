#ifndef EM1_HPP
#define EM1_HPP

#include "block.hpp"
#include <QTimer>
#include <QWidget>


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
    //deletes block with given index
    void _deleteBlockAt(int index);
    void _examineUnder(QPointF pos);
    BlockPtr _getHoveredBlock();
    // returns selected block index, -1 if none selected
    int _getSelectedBlockIndex();

    Block* _getWithPendingConnection();

    QImage mGrid;
    QPoint mGridPosition;
    QVector<std::shared_ptr<Block>> mBlocks;

    bool mScreenGrabbed{false};
    QPoint mGridGrabOffset;

    //private add block method to set up connects
    QTimer mRefreshTimer;

};

#endif // EM1_HPP
