#ifndef BLOCKCONNECTION_HPP
#define BLOCKCONNECTION_HPP

#include "block.hpp"

#include <QObject>
#include <QPainterPath>

class BlockConnection : public QObject
{
    Q_OBJECT
public:
    explicit BlockConnection(QObject *parent = nullptr);

    void draw(QPainter&);
signals:

private:
    Block* mFrom;
    Block* mTo;
    QPointF mEndPoint;

    QPainterPath mPath;

};

#endif // BLOCKCONNECTION_HPP
