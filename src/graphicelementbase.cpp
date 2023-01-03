#include "graphicelementbase.h"
#include "QDebug"


GraphicElementBase::GraphicElementBase(float x, float y, float w, float h, GraphicElementBase *parent)
    : InputListener(parent), mRect(x,y,w,h), mParent(parent) {}

GraphicElementBase::GraphicElementBase()
{

}

GraphicElementBase::~GraphicElementBase()
{
}

bool GraphicElementBase::mouseMoveEvent(QMouseEvent *event) {
    mHovered = mRect.contains(event->pos());

    for(int i = 0; i < mChildren.size(); i++) {
        if (mChildren[i]->mouseMoveEvent(event))
            return true; // if child handled - interrupt
    }

    return false; // ok, we did nothing yet
}

bool GraphicElementBase::mousePressEvent(QMouseEvent *event) {
    for(int i = 0; i < mChildren.size(); i++) {
        if (mChildren[i]->mousePressEvent(event))
            return true; // if child handled - interrupt
    }

    return false;
}

bool GraphicElementBase::mouseReleaseEvent(QMouseEvent *event) {
    for(int i = 0; i < mChildren.size(); i++) {
        if (mChildren[i]->mouseReleaseEvent(event))
            return true; // if child handled - interrupt
    }
    return false;
}

bool GraphicElementBase::mouseDoubleClickEvent(QMouseEvent *event) {
    for(int i = 0; i < mChildren.size(); i++) {
        if (mChildren[i]->mouseDoubleClickEvent(event))
            return true; // if child handled - interrupt
    }
    return false;
}

bool GraphicElementBase::wheelEvent(QWheelEvent *event) {
    for(int i = 0; i < mChildren.size(); i++) {
        if (mChildren[i]->wheelEvent(event))
            return true; // if child handled - interrupt
    }
    return false;
}

void GraphicElementBase::draw(QPainter &) {}

GraphicElementBase *GraphicElementBase::exammineUnder(QPointF pos)
{
    for (int i = 0; i < mChildren.size(); i++) {
        auto layer = mChildren[i]->exammineUnder(pos);
        if (layer)
            return layer;
    }

    if (mHovered)
        return this;
    return nullptr;
}

QRectF GraphicElementBase::getRect() const {return mRect;}

bool GraphicElementBase::isHovered() const
{
    return mHovered;
}

void GraphicElementBase::setPosition(QPointF pos) {mRect.translate(pos-mRect.topLeft());}

void GraphicElementBase::_addChild(GraphicElementBase *newChild) {
    mChildren.append(newChild);
    newChild->_setParent(this);
}

void GraphicElementBase::_removeChild(GraphicElementBase *child)
{
    for (int i = 0; i < mChildren.size(); i++) {
        if (mChildren[i] == child) {
            mChildren.remove(i);
        }
    }
}

void GraphicElementBase::_setParent(GraphicElementBase *parent)
{
    mParent = parent;
}
