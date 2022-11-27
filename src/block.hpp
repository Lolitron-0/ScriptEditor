#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

class Block : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QColor shadowColor MEMBER mShadowColor READ getShadowColor WRITE setShadowColor)
    Q_PROPERTY(QPointF liftUpDelta MEMBER mLiftUpDelta READ getLiftUpDelta WRITE setLiftUpDelta)

    Block(int x, int y);

    void draw(QPainter& painter);

public:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    const QColor &getShadowColor() const;
    void setShadowColor(const QColor &newShadowColor);

    QPointF getLiftUpDelta() const;
    void setLiftUpDelta(QPointF newLiftUpDelta);

signals:
    void requestRepaint();

private:
    QRectF mRect;
    QLinearGradient mBotGradient;
    QBrush mBrush;
    QSequentialAnimationGroup mGrabAnim;
    QPointF mLiftUpDelta;
    QColor mShadowColor;

    bool mSelected{false};
    bool mGrabbed{false};
    QPointF mGrabDelta;

private:
    static QColor selectionColor;
    static QColor fillColor;
    static const int roundRadius = 6;
    static const int selectionWidth = 3;
};


#endif // BLOCK_HPP
