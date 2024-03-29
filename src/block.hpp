#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "blockconnection.hpp"
#include "blockconnectionpin.hpp"
#include "graphicelementbase.h"

#include <QDataStream>
#include <QMouseEvent>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTextEdit>


class Block : public GraphicElementBase
{
    Q_OBJECT
public:
    Q_PROPERTY(QColor shadowColor MEMBER mShadowColor READ getShadowColor WRITE setShadowColor)
    Q_PROPERTY(QPointF liftUpDelta MEMBER mLiftUpDelta READ getLiftUpDelta WRITE setLiftUpDelta)

    // creates block with center at (x, y)
    Block(float x, float y, QWidget* parent ,int id);
    Block(const Block&);
    ~Block();

    Block& operator=(const Block&);
    Block& operator=(Block&&);

    void draw(QPainter& painter) override;
    // used to draw some objects that should always be on top (i. e. connections)
    void drawFrontObjects(QPainter& painter);

public:

    bool mousePressEvent(QMouseEvent *event) override;
    bool mouseReleaseEvent(QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QMouseEvent *event) override;
    bool mouseMoveEvent(QMouseEvent *event) override;

    void grab(QPoint pos);
    void release();
    void connectTo(Block* block);
    void disconnectFrom(Block* block);
    void deletePendingConnection();
    // checks if this block is connected to the other and vice versa
    bool alreadyConnected(Block*);
    // processes front line elements (all-in-one)
    void processFrontLine(InputListener::EventType type, QMouseEvent* );
    void initQWidgets(QWidget* parent);

    int getId() const;

    const QColor &getShadowColor() const;
    void setShadowColor(const QColor &newShadowColor);

    bool isSelected() const;
    void setSelected(bool b);

    QPointF getLiftUpDelta() const;
    void setLiftUpDelta(QPointF newLiftUpDelta);

    BlockConnectionPin &getPin();
    QPointF getPinCenter() const;
    bool hasPendingConnection() const;

    //returns ids of blocks it is connected with (only out connections) for save/load purposes
    QVector<int> getTiedIds();

signals:
    void startContiniousRepaint();
    void stopContiniousRepaint();

private:

    QRectF _getDrawRect();

    int mId;

    QLinearGradient mBotGradient;
    QBrush mBrush;
    QParallelAnimationGroup mGrabAnim;
    QPointF mLiftUpDelta;
    QColor mShadowColor;

    bool mSelected{false};
    bool mGrabbed{false};
    QPointF mGrabDelta;


    BlockConnectionPin mPin;
    QVector<std::shared_ptr<BlockConnection>> mOutConnections;
    bool mPendingConnection{false};

    // contents
    std::shared_ptr<QTextEdit> mEdit;
    QPainterPath mTitlePath;
    QFont mTextFont;
    QFont mTextEditFont;
    QString mTitle;

private:
    static const int defaultWidth = 200;
    static const int defaultHeight = 100;

    static QColor selectionColor;
    static QColor fillColor;
    static const int roundRadius = 6;
    static const int selectionWidth = 2;

private:
    friend QDataStream& operator<<(QDataStream& out, const Block& block);
    friend QDataStream& operator>>(QDataStream& in, Block& block);
};


#endif // BLOCK_HPP
