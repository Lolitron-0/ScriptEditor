#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "blockconnection.hpp"
#include "blockconnectionpin.hpp"
#include "graphicelementbase.h"

#include <QMouseEvent>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

class Block : public GraphicElementBase
{
    Q_OBJECT
public:
    Q_PROPERTY(QColor shadowColor MEMBER mShadowColor READ getShadowColor WRITE setShadowColor)
    Q_PROPERTY(QPointF liftUpDelta MEMBER mLiftUpDelta READ getLiftUpDelta WRITE setLiftUpDelta)

    Block(float x, float y);
    ~Block();

    void draw(QPainter& painter) override;

public:
    bool mousePressEvent(QMouseEvent *event) override;
    bool mouseReleaseEvent(QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QMouseEvent *event) override;
    bool mouseMoveEvent(QMouseEvent *event) override;

    const QColor &getShadowColor() const;
    void setShadowColor(const QColor &newShadowColor);

    QPointF getLiftUpDelta() const;
    void setLiftUpDelta(QPointF newLiftUpDelta);

    BlockConnectionPin &getPin();

signals:
    void startContiniousRepaint();
    void stopContiniousRepaint();

private:

    QRectF _getDrawRect();

    QLinearGradient mBotGradient;
    QBrush mBrush;
    QParallelAnimationGroup mGrabAnim;
    QPointF mLiftUpDelta;
    QColor mShadowColor;

    bool mSelected{false};
    bool mGrabbed{false};
    QPointF mGrabDelta;


    BlockConnectionPin mPin;
    QVector<QSharedPointer<BlockConnection>> mOutputConnections;

private:
    static QColor selectionColor;
    static QColor fillColor;
    static const int roundRadius = 6;
    static const int selectionWidth = 2;
};


#endif // BLOCK_HPP
