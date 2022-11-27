#include "em1.hpp"

#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QPainterPath>


Em1::Em1(QWidget *parent)
    :QWidget(parent), block(100,200)
{
   setGeometry(parent->geometry());
   mGraph = new Graph("ksdjgfk", QPoint(0,100));
   mCurrentNode = mGraph;
   setFocusPolicy(Qt::StrongFocus);
   mRefreshTimer.start(5);
   connect(&mRefreshTimer, &QTimer::timeout, this, [this](){
       this->repaint();
   });
}

void Em1::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    block.draw(painter);


    QPainterPath textPath;
    //renderGraph(&textPath, mGraph);
    painter.drawPath(textPath);
}

void Em1::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        mTempNode.pos = QPoint(0,mCurrentNode->pos.y()+10);
        mCurrentNode->children.append(new Graph(mTempNode));
        mTempNode = Graph();
    }
    else {
        mTempNode.line.append(event->text());
    }
}

void Em1::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    block.mousePressEvent(event);
    repaint();
}

void Em1::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    block.mouseReleaseEvent(event);
    repaint();
}

void Em1::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);

    block.mouseDoubleClickEvent(event);
    repaint();
}

void Em1::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    block.mouseMoveEvent(event);

    repaint();
}

void Em1::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
    repaint();

}

void Em1::renderGraph(QPainterPath *path, Graph* current)
{
    QFont f("Consolas", 12);
    path->addText(current->pos, f, current->line);
    for (int i = 0; i<current->children.size(); i++) {
        renderGraph(path, current->children[i]);
    }
}
