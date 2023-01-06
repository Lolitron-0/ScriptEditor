#include "mainwindow.h"
#include "qdebug.h"
#include "ui_mainwindow.h"
#include <QToolBar>

#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    mMainEditor = new Em1(this);
    mMainEditor->setVisible(true);

    initMenuBar();
    menuBar()->raise();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenuBar()
{

    auto fileMenu = menuBar()->addMenu("File");
    auto newAction = fileMenu->addAction("New");
    connect(newAction, &QAction::triggered, this, [this](){
        mMainEditor->reset();
    });

    auto saveAction = fileMenu->addAction("Save");
    connect(saveAction, &QAction::triggered, this, [this](){
        QFile saveFile("./adhshba.ass");
        if (!saveFile.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to open save file on save";
        }

        QDataStream out(&saveFile);

        mMainEditor->save(out);

        saveFile.close();
    });

    auto loadAction = fileMenu->addAction("Load");
    connect(loadAction, &QAction::triggered, this, [this](){
        mMainEditor->reset();

        QFile saveFile("./adhshba.ass");
        if (!saveFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open save file on load";
        }

        QDataStream out(&saveFile);

        mMainEditor->load(out);

        saveFile.close();
    });
}

