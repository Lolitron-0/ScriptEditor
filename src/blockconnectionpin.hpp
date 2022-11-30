#ifndef BLOCKCONNECTIONPIN_HPP
#define BLOCKCONNECTIONPIN_HPP

#include "graphicelementbase.h"

#include <QRectF>



class BlockConnectionPin : public GraphicElementBase
{
public:
    BlockConnectionPin();

public:
    bool mousePressEvent(QMouseEvent *event) override;
    bool mouseReleaseEvent(QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QMouseEvent *event) override;

    void draw(QPainter &) override;



private:
};

#endif // BLOCKCONNECTIONPIN_HPP
