#include "block.hpp"
#include <QDebug>

QColor Block::selectionColor = QColor(Qt::green).darker(100);
QColor Block::fillColor = QColor(Qt::red).darker(100);


Block::Block(int x, int y)
    :mRect(x,y,100,60), mGrabAnim(this),
     mShadowColor(Qt::transparent)
{
    mBotGradient.setColorAt(0,fillColor.darker(200));
    mBotGradient.setColorAt(0.1,fillColor);
    mBrush = QBrush(mBotGradient);

    QPropertyAnimation* shadowOnAnim = new QPropertyAnimation(this, "shadowColor", this);
    shadowOnAnim->setDuration(150);
    shadowOnAnim->setStartValue(QColor(0,0,0,0));
    shadowOnAnim->setEndValue(QColor(0,0,0,150));

    QPropertyAnimation* liftUpAnim = new QPropertyAnimation(this, "liftUpDelta", this);
    liftUpAnim->setDuration(100);
    liftUpAnim->setStartValue(QPointF(0,0));
    liftUpAnim->setEndValue(QPointF(7,-7));

    mGrabAnim.addAnimation(shadowOnAnim);
    mGrabAnim.addAnimation(liftUpAnim);

    connect(&mGrabAnim, &QAnimationGroup::stateChanged, this, [this](QAbstractAnimation::State newState, QAbstractAnimation::State){
        if (newState == QAbstractAnimation::Stopped)
            emit stopContiniousRepaint();
        else if (newState == QAbstractAnimation::Running)
            emit startContiniousRepaint();
    });
}

void Block::draw(QPainter &painter)
{
    mBotGradient.setStart(mRect.bottomLeft());
    mBotGradient.setFinalStop(mRect.topLeft());
    mBrush = QBrush(mBotGradient);
    painter.setPen(Qt::transparent);

    //shadow
    painter.setBrush(mShadowColor);
    painter.drawRoundedRect(mRect.translated(QPoint()), roundRadius, roundRadius);

    painter.setBrush(mBrush);
    painter.drawRoundedRect(
                mRect.translated(mLiftUpDelta), roundRadius, roundRadius); //fixme: double painting

    if(mSelected){ //sel outline
        auto pen =  QPen(selectionColor);
        pen.setWidth(selectionWidth);
        painter.setPen(pen);
        painter.setBrush(Qt::transparent);
        painter.drawRoundedRect(mRect.translated(mLiftUpDelta), roundRadius, roundRadius);
    }
}

void Block::mousePressEvent(QMouseEvent *event)
{
    if (mRect.contains(event->pos())) {
        mGrabbed = true;
        mSelected = true;
        mGrabAnim.setDirection(QAnimationGroup::Forward);
        mGrabAnim.start();
        mGrabDelta = event->pos() - mRect.topLeft();
    }
    else
        mSelected = false;
}

void Block::mouseReleaseEvent(QMouseEvent *event)
{
    mGrabbed = false;
    mGrabDelta = QPointF(0.,0.);
    mGrabAnim.setDirection(QAnimationGroup::Backward);
    if(mShadowColor.alpha()>0)
        mGrabAnim.start();
}

void Block::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Block::mouseMoveEvent(QMouseEvent *event)
{
    if (mGrabbed){
        //mRect.setX(event->pos().x() + mGrabDelta.x());
        //mRect.setY(event->pos().y() + mGrabDelta.y());
        mRect.translate(event->pos()-mRect.topLeft()-mGrabDelta);
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
}

QPointF Block::getLiftUpDelta() const
{
    return mLiftUpDelta;
}

void Block::setLiftUpDelta(QPointF newLiftUpDelta)
{
    mLiftUpDelta = newLiftUpDelta;
}
