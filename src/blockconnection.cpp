#include "blockconnection.hpp"
#include "block.hpp"
#include "qdebug.h"
#include <cmath>

QColor BlockConnection::connectionColor = QColor(Qt::lightGray);

BlockConnection::BlockConnection(Block *start)
    : BlockConnection(start, nullptr)
{

}

BlockConnection::BlockConnection(Block* start, Block* target)
    : GraphicElementBase(0,0,0,0), mFrom(start), mTo(target)
{
    mPath = QPainterPath(start->getPinCenter());
    mPen = QPen(Qt::white);
    mPen.setColor(connectionColor);
    mPen.setWidth(2);
}

BlockConnection::BlockConnection(const BlockConnection &b)
    : BlockConnection(b.mFrom, b.mTo)
{

}

bool BlockConnection::mouseMoveEvent(QMouseEvent *event)
{
    GraphicElementBase::mouseMoveEvent(event);

    mHovered = _isHovered(event->pos());

    mPen.setWidth(mHovered ? 4 : 2);

    mPath.clear();
    mPath.moveTo(mFrom->getPinCenter());
    if (!mTo) {
        mPath.lineTo(event->pos());
        mPen.setStyle(Qt::PenStyle::DashLine);
    }

    return false;
}

bool BlockConnection::mousePressEvent(QMouseEvent *event)
{
    GraphicElementBase::mousePressEvent(event);

    if (mHovered) {
        switch (event->button()) {
        case Qt::MouseButton::LeftButton:
            //todo: add pins
            break;
        case Qt::MouseButton::RightButton:
            //disconnect
            mFrom->disconnectFrom(mTo);
            break;
        }
    }

    return mHovered;
}

void BlockConnection::draw(QPainter &painter)
{
    if (mTo) {
        mPath.clear();
        mPath.moveTo(mFrom->getPinCenter());
        mPath.lineTo(mTo->getPinCenter());
        // archived since hover is too complicated ))
//        auto vec = mTo->getPinCenter() - mFrom->getPinCenter();
//        mPath.cubicTo( // bezier curves with control points on 1/3 and 2/3 of the vector + 90 deg rotated vector
//                   mFrom->getPinCenter() + 0.33*vec + QPointF((0.33*vec).y(), -(0.33*vec).x()),
//                   mFrom->getPinCenter() + 0.67*vec + QPointF(-(0.33*vec).y(), (0.33*vec).x()),
//                   mTo->getPinCenter());
    }
    painter.setBrush(Qt::transparent);
    painter.setPen(mPen);
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

float BlockConnection::_isHovered(QPoint point) const
{
    if (!mTo) return false;

    QVector2D v(mFrom->getPinCenter());
    QVector2D w(mTo->getPinCenter());
    QVector2D p(point);
    float t = std::max(0.f, std::min(1.f, QVector2D::dotProduct(p - v, w - v) / (v-w).lengthSquared()));
    QVector2D proj = v + t * (w-v);
    return proj.distanceToPoint(p) <= hoverDistance;
}

BlockConnection &BlockConnection::operator=(BlockConnection &&b)
{
    std::swap(mFrom, b.mFrom);
    std::swap(mTo, b.mTo);
    return *this;
}
