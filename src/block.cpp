#include "block.hpp"

QColor Block::selectionColor = QColor(Qt::green).darker(100);
QColor Block::fillColor = QColor(Qt::red).darker(100);


Block::Block(int x, int y)
    :mRect(x,y,100,60), mGrabAnim(this),
     mShadowColor(Qt::transparent)
{
    mBotGradient.setColorAt(0,fillColor.darker(200));
    mBotGradient.setColorAt(0.1,fillColor);
    mBrush = QBrush(mBotGradient);

    QPropertyAnimation* shadowOn = new QPropertyAnimation(this, "shadowColor", this);
    shadowOn->setDuration(400);
    shadowOn->setStartValue(QColor(0,0,0,0));
    shadowOn->setEndValue(QColor(0,0,0,150));


    QPropertyAnimation* shadowOff = new QPropertyAnimation(this, "shadowColor", this);
    shadowOff->setDuration(600);
    shadowOff->setStartValue(QColor(0,0,0,150));
    shadowOff->setEndValue(QColor(0,0,0,0));

    mGrabAnim.addAnimation(shadowOn);
}

void Block::draw(QPainter &painter)
{
    mBotGradient.setStart(mRect.bottomLeft());
    mBotGradient.setFinalStop(mRect.topLeft());
    mBrush = QBrush(mBotGradient);
    painter.setPen(Qt::transparent);

    painter.setBrush(mShadowColor);
    painter.drawRoundedRect(mRect.translated(QPoint(-10,10)), roundRadius, roundRadius);

    painter.setBrush(mBrush);
    painter.drawRoundedRect(mRect, roundRadius, roundRadius);

    if(mSelected){
        auto pen =  QPen(selectionColor);
        pen.setWidth(selectionWidth);
        painter.setPen(pen);
        painter.drawRoundedRect(mRect, roundRadius, roundRadius);
    }
}

void Block::mousePressEvent(QMouseEvent *event)
{
    if (mRect.contains(event->pos())) {
        mGrabbed = true;
        mSelected = true;
        mGrabAnim.setDirection(QAnimationGroup::Forward);
        mGrabAnim.start();
        mGrabDelta = mRect.topLeft() - event->pos();
    }
    else
        mSelected = false;
}

void Block::mouseReleaseEvent(QMouseEvent *event)
{
    mGrabbed = false;
    mGrabAnim.setDirection(QAnimationGroup::Backward);
    mGrabAnim.start();
}

void Block::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Block::mouseMoveEvent(QMouseEvent *event)
{
    if (mGrabbed){
        mRect.translate(event->pos()-mRect.topLeft()+mGrabDelta);
    }
}

const QColor &Block::getShadowColor() const
{
    return mShadowColor;
}

void Block::setShadowColor(const QColor &newShadowColor)
{
    if (mShadowColor == newShadowColor)
        return;
    mShadowColor = newShadowColor;
    emit shadowColorChanged();
}
