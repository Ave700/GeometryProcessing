#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    resize(QDesktopWidget().availableGeometry(this).size() * .7);
    ui->setupUi(this);
    glWindow = new Window(this);

    ui->gridLayout->addWidget(glWindow);
    //setCentralWidget(glWindow);
createActions();
createMenus();
setWindowTitle(tr("Contour"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open existing model file."));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
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

void MainWindow::open(){
const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../models");
if (!fileName.isEmpty()){
    glWindow->setContourFilePath(fileName.toLocal8Bit().constData());
    qInfo(fileName.toLocal8Bit().constData());
}
}
