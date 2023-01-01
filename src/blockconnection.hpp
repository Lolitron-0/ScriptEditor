#ifndef BLOCKCONNECTION_HPP
#define BLOCKCONNECTION_HPP


#include "graphicelementbase.h"

#include <QObject>
#include <QPainterPath>

class Block;

class BlockConnection : public GraphicElementBase
{
public:
    explicit BlockConnection();

    bool mouseMoveEvent(QMouseEvent *) override;

    void draw(QPainter &) override;

    void init(QPointF start);

signals:

private:
    QPointF mStartPoint;
    QPointF mEndPoint;

    QPainterPath mPath;
};

#endif // BLOCKCONNECTION_HPP
