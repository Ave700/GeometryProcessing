#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopWidget>
//---------This file is to change settings on the overall window, the menu bar functionality is containeed here
//
//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//glWindow really contains all the UI elements. To Adjust go into window.cpp and add widgets there.
    //--- Window initializing
    resize(QDesktopWidget().availableGeometry(this).size() * .7);
    ui->setupUi(this);
    glWindow = new Window(this);
    setCentralWidget(glWindow);
createActions();
//createMenus();
//setWindowTitle(tr("Contour"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    //openAct = new QAction(tr("&Open"), this);
    //openAct->setShortcuts(QKeySequence::Open);
    //openAct->setStatusTip(tr("Open existing model file."));
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
}
#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction(openAct);

    //menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

//FUnction called when file open is presssed
void MainWindow::open(){
const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../models");
if (!fileName.isEmpty()){
    glWindow->setContourFilePath(fileName.toLocal8Bit().constData());
    qInfo(fileName.toLocal8Bit().constData());
}
}

