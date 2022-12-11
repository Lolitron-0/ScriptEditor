#ifndef BLOCKCONNECTIONPIN_HPP
#define BLOCKCONNECTIONPIN_HPP

#include "blockconnection.hpp"
#include "graphicelementbase.h"

#include <QRectF>
#include <QSharedPointer>



class BlockConnectionPin : public GraphicElementBase
{
public:
    BlockConnectionPin(Block *parent = nullptr);

public:
    bool mousePressEvent(QMouseEvent *event) override;
    bool mouseReleaseEvent(QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QMouseEvent *event) override;
    bool mouseMoveEvent(QMouseEvent *) override;

    void draw(QPainter &) override;

private:
    //QSharedPointer<BlockConnection> mTempConnection;

};

#endif // BLOCKCONNECTIONPIN_HPP
