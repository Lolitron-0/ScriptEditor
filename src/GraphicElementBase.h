#ifndef GRAPHICELEMENTBASE_H
#define GRAPHICELEMENTBASE_H

#include "inputlistener.hpp"

#include <QSharedPointer>


class GraphicElementBase : public InputListener {

public:

    explicit GraphicElementBase(float x, float y,
                           float w, float h,
                           GraphicElementBase* parent = new GraphicElementBase());
    //Null object
    explicit GraphicElementBase();

    ~GraphicElementBase();


    bool mouseMoveEvent(QMouseEvent *event) override;
    bool mousePressEvent(QMouseEvent *event) override;
    bool mouseReleaseEvent(QMouseEvent *event) override;
    bool mouseDoubleClickEvent(QMouseEvent *event) override;
    bool wheelEvent(QWheelEvent *event) override;

    //drawing is left to client as it is more likely to be in the middle of method
    virtual void draw(QPainter&);

    //checks everything under given position returns null if nothing was found
    GraphicElementBase *exammineUnder(QPointF pos);

    QRectF getRect() const;
    bool isHovered() const;

    bool doSkipSystemHandling() const;
    void setSkipSystemHandling(bool b);

    // Sets the position of rect's top left corner
    void setPosition(QPointF pos);
    QPoint getPosition();

protected:
    void _addChild(GraphicElementBase* newChild);
    void _removeChild(GraphicElementBase* child);
    void _setParent(GraphicElementBase* parent);

    QRectF mRect;
    bool mHovered{false};
    // some elements need to be proceeded by hand (i.e. front line)
    bool mSkipSystemHandling{false};

    GraphicElementBase* mParent;

private:

    QVector<GraphicElementBase*> mChildren;
};

#endif // GRAPHICELEMENTBASE_H
