#include "em1.hpp"
#include "utils.hpp"

#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QPainterPath>
#include <QSharedPointer>


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

    painter.drawImage(mGridPosition, mGrid);

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

    for (int i = mBlocks.size()-1; i >= 0; i--) {
        if (mBlocks[i]->mousePressEvent(event)) {
            if (event->button() == Qt::MouseButton::RightButton) {
                _deleteBlockAt(i);
            }
            return; // if someone handled we interrupt everything
        }
    }

    if (event->button() == Qt::MouseButton::MiddleButton) {
        mScreenGrabbed = true;
        mGridGrabOffset = event->pos() - mGridPosition;
        for (int i = 0 ; i < mBlocks.size(); i++)
            mBlocks[i]->grab(event->pos());
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
                return; // if someone handled during connection we interrupt everything
            }
        }
    }

    if (event->button() == Qt::MouseButton::MiddleButton) {
        mScreenGrabbed = false;
        for (int i = 0 ; i < mBlocks.size(); i++)
            mBlocks[i]->release();
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

    if (event->button() == Qt::MouseButton::LeftButton)
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

    if (mScreenGrabbed) {
        auto nextPosition = (mGridGrabOffset - event->pos());
        if (nextPosition.x()<0  ||
            nextPosition.y()<0) { //if we are out of bounds
            mGridGrabOffset = event->pos() - mGridPosition;
            for (int i = 0 ; i < mBlocks.size(); i++)
                mBlocks[i]->release();
        }
        else {
            mGridPosition = event->pos() - mGridGrabOffset;
            for (int i = 0 ; i < mBlocks.size(); i++)
                mBlocks[i]->grab(event->pos());
        }
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

void Em1::reset()
{
    auto blocksToDelete = mBlocks.size();
    for (int i = 0; i < blocksToDelete; i++) {
        _deleteBlockAt(0); //deleting 0 block will ignore shifting after removal
    }
}

void Em1::save(QDataStream &stream)
{
    stream << mBlocks.size();
    for (int i = 0; i < mBlocks.size(); i++)
        stream << *mBlocks[i].get();
    for (int i = 0; i < mBlocks.size(); i++) {
        auto cons = mBlocks[i]->getTiedIds();
        stream<<mBlocks[i]->getId();
        stream<<cons;
    }

    stream << IdGenerator::getInstance();
}

void Em1::load(QDataStream &stream)
{
    int blockCount;
    stream >> blockCount;
    for (int i = 0; i < blockCount; i++) {
        Block newBlock(0,0, this, -1);
        stream >> newBlock;
        mBlocks.append(std::make_shared<Block>(std::move(newBlock)));
    }
    for (int i = 0; i < blockCount; i++) {
        int curId;
        stream >> curId;
        auto block = _getById(curId);
        QVector<int> cons;
        stream>>cons;
        foreach (int id, cons) {
            block->connectTo(_getById(id));
        }
    }

    stream >> IdGenerator::getInstance();
}

void Em1::_addBlock(QPoint pos)
{
    auto newBlock = std::make_shared<Block>(pos.x(), pos.y(), this, IdGenerator::getInstance().getNextId());
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

Block* Em1::_getById(int id)
{
    //we assume that mBlocks is stable and ids are added incrementally so binary search can be used
    int l = 0, r = mBlocks.size(), mid;
    bool found = false;
    while (l<=r && !found) {
        mid = (l+r)/2;
        if (mBlocks[mid]->getId() == id) found = true;
        else if (mBlocks[mid]->getId() > id) r = mid-1;
        else l = mid+1;
    }

    return (found ? mBlocks[mid].get() : nullptr);
}
