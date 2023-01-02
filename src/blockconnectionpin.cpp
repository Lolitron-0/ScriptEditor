#include "blockconnectionpin.hpp"
#include "block.hpp"

#include <QPainter>
#include <QDebug>

BlockConnectionPin::BlockConnectionPin(Block *parent)
    :GraphicElementBase(0,0,15,15, parent)
{
}

bool BlockConnectionPin::mousePressEvent(QMouseEvent *event)
{
    GraphicElementBase::mousePressEvent(event);

    if (mHovered) {
        mTempConnection.reset(new BlockConnection(static_cast<Block*>(mParent))); //we guarantee that our parent is Block
        emit startedConnection();
    }

    return mHovered;
}

bool BlockConnectionPin::mouseReleaseEvent(QMouseEvent *event)
{
    GraphicElementBase::mouseReleaseEvent(event);
    return false; // we don't want to break mouse release event chains
}

bool BlockConnectionPin::mouseDoubleClickEvent(QMouseEvent *event)
{
    GraphicElementBase::mouseDoubleClickEvent(event);
    return mHovered;
}

bool BlockConnectionPin::mouseMoveEvent(QMouseEvent *event)
{
    GraphicElementBase::mouseMoveEvent(event);
    if (mTempConnection.get())
        mTempConnection->mouseMoveEvent(event);
    return mHovered;
}

void BlockConnectionPin::draw(QPainter &painter)
{

    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::black);
    painter.drawRoundedRect(mRect, 3,3);

    if(mHovered)
        painter.setBrush(Qt::GlobalColor::lightGray);
    else
        painter.setBrush(Qt::white);
    painter.setPen(Qt::transparent);
    painter.drawEllipse(mRect.center(), 5,5);
    if (mTempConnection.get())
        mTempConnection->draw(painter);
}

void BlockConnectionPin::finishConnection()
{
    mTempConnection.reset();
}

