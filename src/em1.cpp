#include "em1.hpp"
#include "qlayout.h"

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

    mGrid = QImage(":/images/grid.jpg");
    mGrid.setDevicePixelRatio(2);

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

    for (int i = 0; i < mBlocks.size(); i++) {
        mBlocks[i]->drawFrontObjects(painter);
    }
}

void Em1::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key::Key_Delete) {
        auto selected = _getSelectedBlockIndex();
        if (selected != -1)
            _deleteBlockAt(selected);
    }
    else {
    }
}

void Em1::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    for (int i = 0; i<mBlocks.size(); i++) // deselect all
        mBlocks[i]->setSelected(false);

    for (int i = 0; i<mBlocks.size(); i++) {
        if (mBlocks[i]->mousePressEvent(event)) {
            if (event->button() == Qt::MouseButton::RightButton) {
                _deleteBlockAt(i);
            }
            return; // if someone handled we interrupt everything
        }
    }

    //front line
    for (int i = 0; i<mBlocks.size(); i++)
        mBlocks[i]->processFrontLine(InputListener::EventType::MousePress, event);

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

    if (blockWithPendingConnection) { // failed to connect
        _addBlock(event->pos());
        blockWithPendingConnection->connectTo(mBlocks.last().get());
    }

    //front line
    for (int i = 0; i<mBlocks.size(); i++)
        mBlocks[i]->processFrontLine(InputListener::EventType::MouseRelease, event);

    repaint();
}

void Em1::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);

    for (int i = 0; i<mBlocks.size(); i++) {
        if (mBlocks[i]->mouseDoubleClickEvent(event))
            return; // if someone handled we interrupt everything
    }

    //front line
    for (int i = 0; i<mBlocks.size(); i++)
        mBlocks[i]->processFrontLine(InputListener::EventType::MouseDoubleClick, event);

    _addBlock(event->pos());
    repaint();
}

void Em1::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    auto blockWithPendingConnection = _getWithPendingConnection();
    for (int i = 0; i<mBlocks.size(); i++) {
        if (mBlocks[i]->mouseMoveEvent(event) &&
                !blockWithPendingConnection &&
                mBlocks[i]->isSelected()) // priority to selected
            return; // if someone handled we interrupt everything, do not interrupt block with pending connection
    }

    //front line
    for (int i = 0; i<mBlocks.size(); i++)
        mBlocks[i]->processFrontLine(InputListener::EventType::MouseMove, event);

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
    newBlock->setTextEditPtr(std::make_shared<QTextEdit>(this)); // due to my stupidness we need to init QWidgets outside
    mBlocks.push_back(newBlock);
}

void Em1::_deleteBlockAt(int index)
{
    for (int i = 0; i < mBlocks.size(); i++) {
        if (i != index) {
            mBlocks[i]->disconnectFrom(mBlocks[index].get());
        }
    }

    mBlocks.remove(index);
}

Em1::BlockPtr Em1::_getHoveredBlock()
{
    for (int i = 0; i < mBlocks.size(); i++) {
        if (mBlocks[i]->isHovered())
            return mBlocks[i];
    }
    return nullptr;
}

int Em1::_getSelectedBlockIndex()
{
    for (int i = 0; i < mBlocks.size(); i++) {
        if (mBlocks[i]->isSelected())
            return i;
    }
    return -1;
}

Block *Em1::_getWithPendingConnection()
{
    for (int i = 0; i < mBlocks.size(); i++) {
        if (mBlocks[i]->hasPendingConnection())
            return mBlocks[i].get();
    }
    return nullptr;
}
