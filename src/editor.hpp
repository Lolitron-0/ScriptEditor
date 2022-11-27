#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <QTextEdit>
#include <QWidget>

class Editor : public QTextEdit
{
public:
    Editor(QWidget* parent = nullptr);

private slots:
    void highlightCurrentLine();


protected:
    void wheelEvent(QWheelEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    void setAllFontSize(qreal s);

};

#endif // EDITOR_HPP
