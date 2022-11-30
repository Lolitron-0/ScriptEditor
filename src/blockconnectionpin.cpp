#include "blockconnectionpin.hpp"

#include <QPainter>

BlockConnectionPin::BlockConnectionPin()
    :GraphicElementBase(0,0,15,15)
{

}

bool BlockConnectionPin::mousePressEvent(QMouseEvent *event)
{
    GraphicElementBase::mousePressEvent(event);

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
}
