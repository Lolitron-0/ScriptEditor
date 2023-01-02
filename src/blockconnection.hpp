#ifndef BLOCKCONNECTION_HPP
#define BLOCKCONNECTION_HPP


#include "graphicelementbase.h"

#include <QObject>
#include <QPainterPath>
#include <QPen>

class Block;

class BlockConnection : public GraphicElementBase
{
public:
    explicit BlockConnection(Block* start);
    explicit BlockConnection(Block* start, Block* end);
    BlockConnection(const BlockConnection& b);

    bool mouseMoveEvent(QMouseEvent *) override;

    void draw(QPainter &) override;

    Block* to();
    Block* from();

    BlockConnection& operator=(const BlockConnection& b);
    BlockConnection& operator=(BlockConnection&& b);

private:
    Block* mFrom;
    Block* mTo;

    QPainterPath mPath;
    QPen mPen;
};

#endif // BLOCKCONNECTION_HPP
