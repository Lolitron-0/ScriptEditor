#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mMainEditor = new Em1(this);
    this->layout()->addWidget(mMainEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

