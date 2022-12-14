#ifndef BLOCKCONNECTIONPIN_HPP
#define BLOCKCONNECTIONPIN_HPP

#include "blockconnection.hpp"
#include "graphicelementbase.h"

#include <QRectF>
#include <QSharedPointer>



class BlockConnectionPin : public GraphicElementBase
{
    Q_OBJECT
public:
    BlockConnectionPin(Block *parent = nullptr);

public:
    bool mousePressEvent(QMouseEvent *event) override;
    bool mouseReleaseEvent(QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QMouseEvent *event) override;
    bool mouseMoveEvent(QMouseEvent *) override;

    void draw(QPainter &) override;

    //finishes pending connection
    void finishConnection();

signals:
    void startedConnection();

private:
    std::shared_ptr<BlockConnection> mTempConnection;

};

#endif // BLOCKCONNECTIONPIN_HPP
