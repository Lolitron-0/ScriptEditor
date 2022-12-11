#include "block.hpp"
#include <QDebug>

QColor Block::selectionColor = QColor(Qt::green).darker(100);
QColor Block::fillColor = QColor(Qt::red).darker(100);


Block::Block(float x, float y)
    :GraphicElementBase(x,y,100,60), mGrabAnim(this),
     mShadowColor(Qt::transparent), mPin(this)
{
    mBotGradient.setColorAt(0,fillColor.lighter(150));
    mBotGradient.setColorAt(0.2,fillColor);
    mBrush = QBrush(mBotGradient);

    QPropertyAnimation* shadowOnAnim = new QPropertyAnimation(this, "shadowColor", this);
    shadowOnAnim->setDuration(100);
    shadowOnAnim->setStartValue(QColor(100,100,100,0));
    shadowOnAnim->setEndValue(QColor(100,100,100,255));

    QPropertyAnimation* liftUpAnim = new QPropertyAnimation(this, "liftUpDelta", this);
    liftUpAnim->setDuration(70);
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

    //_addChild(&mPin);
}

Block::~Block()
{

}

void Block::draw(QPainter &painter)
{
    mBotGradient.setStart(mRect.bottomLeft());
    mBotGradient.setFinalStop(mRect.topLeft());
    if (mHovered) mBotGradient.setColorAt(0, fillColor.lighter(170));
    else mBotGradient.setColorAt(0, fillColor.lighter(150));
    mBrush = QBrush(mBotGradient);
    painter.setPen(Qt::transparent);

    //shadow
    painter.setBrush(mShadowColor);
    painter.drawRoundedRect(mRect, roundRadius, roundRadius);

    painter.setBrush(mBrush);
    painter.drawRoundedRect(
                _getDrawRect(), roundRadius, roundRadius);

    //shifting pin properly
    mPin.setPosition(_getDrawRect().topRight()-
                     QPointF( //offset
                         mPin.getRect().width() + 5, //magic number whatever
                         -_getDrawRect().height()/2 + mPin.getRect().height()/2));
    mPin.draw(painter);

    if(mSelected){ //sel outline
        auto pen =  QPen(selectionColor);
        pen.setWidth(selectionWidth);
        painter.setPen(pen);
        painter.setBrush(Qt::transparent);
        painter.drawRoundedRect(_getDrawRect(), roundRadius, roundRadius);
    }
}

bool Block::mousePressEvent(QMouseEvent *event)
{
    if (GraphicElementBase::mousePressEvent(event)) return true;

    if (mHovered){
        mGrabbed = true;
        mSelected = true;
        mGrabAnim.setDirection(QAnimationGroup::Forward);
        mGrabAnim.start();
        mGrabDelta = event->pos() - mRect.topLeft();
        return true;
    }
    else
        mSelected = false;
    return false;
}

bool Block::mouseReleaseEvent(QMouseEvent *event)
{
    if (GraphicElementBase::mouseReleaseEvent(event)) return true;

    mGrabbed = false;
    mGrabDelta = QPointF(0.,0.);
    mGrabAnim.setDirection(QAnimationGroup::Backward);
    if(mShadowColor.alpha()>0)
        mGrabAnim.start();

    return mHovered;
}

bool Block::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (GraphicElementBase::mouseDoubleClickEvent(event)) return true;
    return mHovered;
}

bool Block::mouseMoveEvent(QMouseEvent *event)
{
    if (GraphicElementBase::mouseMoveEvent(event)) return true;

    if (mGrabbed){
        mRect.translate(event->pos()-mRect.topLeft()-mGrabDelta);
    }

    return mHovered;
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

BlockConnectionPin& Block::getPin()
{
    return mPin;
}

QRectF Block::_getDrawRect()
{
    return mRect.translated(mLiftUpDelta);
}
