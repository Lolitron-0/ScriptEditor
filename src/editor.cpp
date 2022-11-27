#include "editor.hpp"
#include <QDebug>
#include <QWheelEvent>
#include <QTextBlock>
#include <QPainter>

Editor::Editor(QWidget *parent)
    :QTextEdit(parent)
{
    connect(this, &Editor::cursorPositionChanged, this, &Editor::highlightCurrentLine);
    setGeometry(0,0,parent->width(), parent->height());
    insertPlainText("//start your script below");
    highlightCurrentLine();
    setAllFontSize(14);
    QTextCharFormat format;
    setCurrentCharFormat(format);
    qDebug()<<document()->firstBlock().blockNumber();
}

void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if(!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::green).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.format.setTextOutline(QPen(QColor(Qt::black)));
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void Editor::wheelEvent(QWheelEvent *event)
{
   if (event->modifiers() & Qt::ControlModifier) {
       setAllFontSize(fontPointSize()+3*(event->angleDelta().y()>0?1:-1));
   }
   repaint();
   highlightCurrentLine();
}

void Editor::paintEvent(QPaintEvent *event)
{
    QTextEdit::paintEvent(event);
}

void Editor::setAllFontSize(qreal s)
{
    if (fontPointSize()+s<0) return;
    auto cursor = textCursor();
    selectAll();
    setFontPointSize(s);
    setTextCursor( cursor );
}
