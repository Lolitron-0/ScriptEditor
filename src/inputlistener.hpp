#ifndef INPUTLISTENER_HPP
#define INPUTLISTENER_HPP

#include <QMouseEvent>
#include <QObject>

class InputListener : public QObject
{
    Q_OBJECT
public:
    explicit InputListener(QObject *parent = nullptr) :QObject(parent) {}

    // bool return value is for all inheritors to interrupt (or not) event chains
    virtual bool mousePressEvent(QMouseEvent*){return false;}
    virtual bool mouseReleaseEvent(QMouseEvent*){return false;}
    virtual bool mouseDoubleClickEvent(QMouseEvent*){return false;}
    virtual bool mouseMoveEvent(QMouseEvent*){return false;}
    virtual bool wheelEvent(QWheelEvent*){return false;}

signals:

};

#endif // INPUTLISTENER_HPP
