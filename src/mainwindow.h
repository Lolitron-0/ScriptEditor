#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "em1.hpp"

#include <editor.hpp>
#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Em1* mMainEditor;
};
#endif // MAINWINDOW_H
