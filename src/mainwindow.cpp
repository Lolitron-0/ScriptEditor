#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolBar>

#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMenuBar();

    mMainEditor = new Em1(this);
    mMainEditor->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenuBar()
{
    auto fileMenu = menuBar()->addMenu("File");
    fileMenu->addMenu("New");
}

