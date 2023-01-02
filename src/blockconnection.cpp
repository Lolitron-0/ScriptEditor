#include "blockconnection.hpp"
#include "block.hpp"

BlockConnection::BlockConnection(Block *start)
    : BlockConnection(start, nullptr)
{

}

BlockConnection::BlockConnection(Block* start, Block* target)
    : GraphicElementBase(0,0,0,0), mFrom(start), mTo(target)
{
    mPath = QPainterPath(start->getPinCenter());
    mPen = QPen(Qt::white);
    mPen.setWidth(2);
}

BlockConnection::BlockConnection(const BlockConnection &b)
    : BlockConnection(b.mFrom, b.mTo)
{

}

bool BlockConnection::mouseMoveEvent(QMouseEvent *event)
{
    mPath.clear();
    mPath.moveTo(mFrom->getPinCenter());
    if (!mTo)
        mPath.lineTo(event->pos());
    return false;
}

void BlockConnection::draw(QPainter &painter)
{
    painter.setBrush(Qt::transparent);
    painter.setPen(mPen);
    if (mTo) {
        mPath.clear();
        mPath.moveTo(mFrom->getPinCenter());
        mPath.lineTo(mTo->getPinCenter());
    }
    painter.drawPath(mPath);
}

Block *BlockConnection::to()
{
    return mTo;
}

Block *BlockConnection::from()
{
    return mFrom;
}

BlockConnection &BlockConnection::operator=(const BlockConnection &b)
{
    BlockConnection tmp(b);
    std::swap(mFrom, tmp.mFrom);
    std::swap(mTo, tmp.mTo);
    return *this;
}

BlockConnection &BlockConnection::operator=(BlockConnection &&b)
{
    std::swap(mFrom, b.mFrom);
    std::swap(mTo, b.mTo);
    return *this;
}
