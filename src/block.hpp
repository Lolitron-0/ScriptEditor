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
    Block(const Block&);
    ~Block();

    Block& operator=(const Block&);
    Block& operator=(Block&&);

    void draw(QPainter& painter) override;

public:
    bool mousePressEvent(QMouseEvent *event) override;
    bool mouseReleaseEvent(QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QMouseEvent *event) override;
    bool mouseMoveEvent(QMouseEvent *event) override;

    void connectTo(Block* block);
    void disconnectFrom(Block* block);
    void deletePendingConnection();
    // checks if this block is connected to the other and vice versa
    bool alreadyConnected(Block*);

    const QColor &getShadowColor() const;
    void setShadowColor(const QColor &newShadowColor);

    bool isSelected() const;
    void setSelected(bool b);

    QPointF getLiftUpDelta() const;
    void setLiftUpDelta(QPointF newLiftUpDelta);

    BlockConnectionPin &getPin();
    QPointF getPinCenter() const;
    bool hasPendingConnection() const;

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
    QVector<BlockConnection> mOutputConnections;
    bool mPendingConnection{false};

private:
    static const int defaultWidth = 100;
    static const int defaultHeight = 60;

    static QColor selectionColor;
    static QColor fillColor;
    static const int roundRadius = 6;
    static const int selectionWidth = 2;
};


#endif // BLOCK_HPP
