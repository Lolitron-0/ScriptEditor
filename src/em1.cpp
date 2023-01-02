#include "em1.hpp"

#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QPainterPath>


Em1::Em1(QWidget *parent)
    :QWidget(parent)
{
    setGeometry(parent->geometry());
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    mGraph = new Graph("ksdjgfk", QPoint(0,100));
    mCurrentNode = mGraph;

    mGrid = QImage(":/images/grid.jpg");

    mRefreshTimer.start(5);
    connect(&mRefreshTimer, &QTimer::timeout, this, [this](){
        this->repaint();
    });

    _addBlock(QPoint(100,100));
    _addBlock(QPoint(350,220));
}

void Em1::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.drawImage(QPoint(), mGrid);

    for (int i = 0; i < mBlocks.size(); i++) {
        mBlocks[i]->draw(painter);
    }

    QPainterPath textPath;
    //renderGraph(&textPath, mGraph);
    painter.drawPath(textPath);
}

void Em1::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        mTempNode.pos = QPoint(0,mCurrentNode->pos.y()+10);
        mCurrentNode->children.append(new Graph(mTempNode));
        mTempNode = Graph();
    }
    else {
        mTempNode.line.append(event->text());
    }
}

void Em1::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    for (int i = 0; i<mBlocks.size(); i++) {
        if (mBlocks[i]->mousePressEvent(event))
            return; // if someone handled we interrupt everything
    }
    repaint();
}

void Em1::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    auto blockWithPendingConnection = _getWithPendingConnection();

    if (blockWithPendingConnection)
        blockWithPendingConnection->deletePendingConnection();

    for (int i = 0; i<mBlocks.size(); i++) {
        if (mBlocks[i]->mouseReleaseEvent(event)) {
            if (blockWithPendingConnection) {
                if (mBlocks[i].get() != blockWithPendingConnection &&  // do not connect with ourselves
                        !mBlocks[i]->alreadyConnected(blockWithPendingConnection)) {
                    blockWithPendingConnection->connectTo(mBlocks[i].get());
                }
            }
            return; // if someone handled we interrupt everything
        }
    }

    repaint();
}

void Em1::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);

    for (int i = 0; i<mBlocks.size(); i++) {
        if (mBlocks[i]->mouseDoubleClickEvent(event))
            return; // if someone handled we interrupt everything
    }

    _addBlock(event->pos());
    repaint();
}

void Em1::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    auto blockWithPendingConnection = _getWithPendingConnection();
    for (int i = 0; i<mBlocks.size(); i++) {
        if (mBlocks[i]->mouseMoveEvent(event) && !blockWithPendingConnection)
            return; // if someone handled we interrupt everything, do not interrupt block with pending connection
    }
    repaint();
}

void Em1::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
    repaint();

}

void Em1::_addBlock(QPoint pos)
{
    auto newBlock = std::make_shared<Block>(pos.x(), pos.y());
    /*
    connect(&*newBlock, &Block::startContiniousRepaint, this, [this]() {
        this->mRefreshTimer.start(5);
        qDebug()<<"on";
    });
    connect(&*newBlock, &Block::stopContiniousRepaint, this, [this]() {
        this->mRefreshTimer.stop();
        qDebug()<<"off";
    });
    */
    mBlocks.push_back(newBlock);
}

Em1::BlockPtr Em1::_getHoveredBlock()
{
    for (int i = 0; i < mBlocks.size(); i++) {
        if (mBlocks[i]->isHovered())
            return mBlocks[i];
    }
    return nullptr;
}

Block *Em1::_getWithPendingConnection()
{
    for (int i = 0; i < mBlocks.size(); i++) {
        if (mBlocks[i]->hasPendingConnection())
            return mBlocks[i].get();
    }
    return nullptr;
}
