#include "blockconnection.hpp"
#include "block.hpp"

BlockConnection::BlockConnection()
    : GraphicElementBase(0,0,0,0)
{

}

bool BlockConnection::mouseMoveEvent(QMouseEvent *event)
{
    mPath.clear();
    mPath.lineTo(event->pos());
    return false;
}

void BlockConnection::draw(QPainter &painter)
{
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::white);
    painter.drawPath(mPath);
}

void BlockConnection::init(Block *parent)
{
    mPath = QPainterPath(parent->getPin().getRect().center());
    mFrom = parent;
}
