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
    Q_PROPERTY(QColor shadowColor MEMBER mShadowColor READ getShadowColor WRITE setShadowColor NOTIFY shadowColorChanged)

    Block(int x, int y);

    void draw(QPainter& painter);

public:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    const QColor &getShadowColor() const;
    void setShadowColor(const QColor &newShadowColor);

signals:
    void shadowColorChanged();
    void requestRepaint();

private:
    QRect mRect;
    QLinearGradient mBotGradient;
    QBrush mBrush;
    QSequentialAnimationGroup mGrabAnim;
    QColor mShadowColor;

    bool mSelected{false};
    bool mGrabbed{false};
    QPoint mGrabDelta;

private:
    static QColor selectionColor;
    static QColor fillColor;
    static const int roundRadius = 6;
    static const int selectionWidth = 3;
};


#endif // BLOCK_HPP
