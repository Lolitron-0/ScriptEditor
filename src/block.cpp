#include "block.hpp"
#include <QDebug>

QColor Block::selectionColor = QColor(Qt::yellow).darker(100);
QColor Block::fillColor = QColor(252,50,50);


Block::Block(float x, float y)
    :GraphicElementBase(x-defaultWidth/2,y-defaultHeight/2,defaultWidth,defaultHeight), mGrabAnim(this),
     mShadowColor(Qt::transparent), mPin(this), mTextFont("Consolas"), mTitle("Story")
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
        if (newState == QAbstractAnimation::Stopped && !mGrabbed)
            emit stopContiniousRepaint();
        else if (newState == QAbstractAnimation::Running)
            emit startContiniousRepaint();
    });

    connect(&mPin, &BlockConnectionPin::startedConnection, this, [this](){
        this->mPendingConnection = true;
    });

    _addChild(&mPin);

    mTextFont.setPixelSize(15);
    mTextFont.setBold(true);
    // text edit setup is in it's set method
    mTextEditFont.setFamily("Consolas");
    mTextEditFont.setPointSize(8);
}

Block::Block(const Block &b)
    :Block(b.getRect().x(), b.getRect().y())
{

}

Block::~Block()
{

}

Block &Block::operator=(const Block &b)
{
    Block tmp(b);
    std::swap(mRect, tmp.mRect);
    return *this;
}

Block &Block::operator=(Block &&b)
{
    std::swap(mRect, b.mRect);
    return *this;
}

void Block::draw(QPainter &painter)
{
    mBotGradient.setStart(_getDrawRect().bottomLeft());
    mBotGradient.setFinalStop(_getDrawRect().topLeft());
    if (mHovered) mBotGradient.setColorAt(0, fillColor.lighter(200));
    else mBotGradient.setColorAt(0, fillColor.lighter(150));
    mBrush = QBrush(mBotGradient);
    painter.setPen(Qt::transparent);

    // shadow
    painter.setBrush(mShadowColor);
    painter.drawRoundedRect(mRect, roundRadius, roundRadius);

    // body
    painter.setPen(Qt::black);
    painter.setBrush(mBrush);
    painter.drawRoundedRect(
                _getDrawRect(), roundRadius, roundRadius);

    // shifting pin properly
    mPin.setPosition(_getDrawRect().topRight()-
                     QPointF( // offset
                         mPin.getRect().width() + 5, // random number whatever
                         -_getDrawRect().height()/2 + mPin.getRect().height()/2));
    mPin.draw(painter);

    // contents

    auto letterSize = mTextFont.pixelSize();
    auto contentCorner = QPoint(_getDrawRect().left()+letterSize/2,
                                _getDrawRect().top()+ 2 * letterSize); //including title

    painter.setPen(Qt::transparent);
    mTitlePath.clear();
    mTitlePath.addText(QPoint(_getDrawRect().center().x() - letterSize/2 * mTitle.length()/2,
                              _getDrawRect().top() + letterSize),
                       mTextFont, mTitle);
    painter.setPen(Qt::black);
    painter.drawLine(
                    QPoint(contentCorner.x(), _getDrawRect().top() + 1.5 * letterSize),
                    QPoint(_getDrawRect().right() - letterSize/2, _getDrawRect().top() + 1.5 * letterSize));
    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    painter.drawPath(mTitlePath);


    mEdit->setGeometry(contentCorner.x(),
                      contentCorner.y(),
                      mPin.getRect().left() - _getDrawRect().left() - letterSize/2 - 5,
                      _getDrawRect().bottom()-contentCorner.y() - letterSize/2
                );

    if(mSelected){ //sel outline
        auto pen =  QPen(selectionColor);
        pen.setWidth(selectionWidth);
        painter.setPen(pen);
        painter.setBrush(Qt::transparent);
        painter.drawRoundedRect(_getDrawRect(), roundRadius, roundRadius);
    }

}

void Block::drawFrontObjects(QPainter &painter)
{
    // connections
    for (int i = 0; i < mOutConnections.size(); i++) {
        mOutConnections[i]->draw(painter);
    }
}

bool Block::mousePressEvent(QMouseEvent *event)
{
    if (GraphicElementBase::mousePressEvent(event)) return true;

    if (mHovered && event->button() == Qt::MouseButton::LeftButton) {
        mGrabbed = true;
        mSelected = true;
        mGrabAnim.setDirection(QAnimationGroup::Forward);
        mGrabAnim.start();
        mGrabDelta = event->pos() - mRect.topLeft();
        return true;
    }
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

void Block::grab(QPoint pos)
{
        mGrabbed = true;
        mGrabDelta = pos - mRect.topLeft();
}

void Block::release()
{
    mGrabbed = false;
    mGrabDelta = QPointF(0.,0.);
}

void Block::connectTo(Block *block)
{
    mOutConnections.append(std::make_shared<BlockConnection>(this, block));
    mOutConnections.last()->setSkipSystemHandling(true); // we want to process them ourselves by hand
    mPin.finishConnection();
    _addChild(mOutConnections.last().get());
    mPendingConnection = false;
}

void Block::disconnectFrom(Block *block)
{
    for (int i = 0; i < mOutConnections.size(); i++) {
        if (mOutConnections[i]->to() == block) {
            _removeChild(mOutConnections[i].get());
            mOutConnections.remove(i);
        }
    }
}

void Block::deletePendingConnection()
{
    mPin.finishConnection();
    mPendingConnection = false;
}

bool Block::alreadyConnected(Block *b)
{
    // other with this
    for (int i = 0; i < b->mOutConnections.size(); i++) {
        if (b->mOutConnections[i]->to() == this)
            return true;
    }

    // this with other
    for (int i = 0; i < mOutConnections.size(); i++) {
        if (mOutConnections[i]->to() == b)
            return true;
    }

    return false;
}

void Block::processFrontLine(EventType type, QMouseEvent* event)
{
    for (int i = 0; i < mOutConnections.size(); i++) {
        switch (type) {
        case InputListener::EventType::MouseMove:
            mOutConnections[i]->mouseMoveEvent(event);
            break;
        case InputListener::EventType::MousePress:
            mOutConnections[i]->mousePressEvent(event);
            break;
        case InputListener::EventType::MouseRelease:
            mOutConnections[i]->mouseReleaseEvent(event);
            break;
        case InputListener::EventType::MouseDoubleClick:
            mOutConnections[i]->mouseDoubleClickEvent(event);
            break;
        default:
            break;
        }
    }
}

void Block::initQWidgets(QWidget *parent)
{
    mEdit = std::make_shared<QTextEdit>(parent);
    mEdit->setFontFamily("Consolas");
    mEdit->setStyleSheet(QString::fromUtf8(R"STYLE(
QTextEdit {
    background-color: rgba(50,50,50,50);
    border-radius: 6px;
    color: white;
}
)STYLE"));
    mEdit->setFont(mTextEditFont);
    mEdit->setVisible(true);
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

bool Block::isSelected() const
{
    return mSelected;
}

void Block::setSelected(bool b)
{
    mSelected = b;
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

QPointF Block::getPinCenter() const
{
    return mPin.getRect().center();
}

bool Block::hasPendingConnection() const
{
    return mPendingConnection;
}

QRectF Block::_getDrawRect()
{
    return mRect.translated(mLiftUpDelta);
}
