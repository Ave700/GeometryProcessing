
#include "contourwidget.h"
#include "window.h"
#include "mainwindow.h"
#include "ui_contourwidget.h"



Window::Window(MainWindow *mw) : mainWindow(mw), ui(new Ui::ContourWidget){

    //contourWidget->setSizePolicy(Preferred);
    //Ui::CentralWidget ui;
    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //QHBoxLayout *container = new QHBoxLayout;
    //QVBoxLayout *optionsPane = new QVBoxLayout;
    ui->setupUi(this);
    contourWidget = new ContourWidget;
    //--- Render options drop down
    ui->GLHolder->addWidget(contourWidget);

    QObject::connect(ui->renderOptions, SIGNAL(activated(int)), this, SLOT(renderSlot(int)));
    QObject::connect(ui->renderHollowBox, SIGNAL(stateChanged(int)), this, SLOT(renderHollowSlot()));

}


void Window::setContourFilePath(std::string filepath){
    contourWidget->setModelFilePath(filepath);
}


void Window::renderSlot(int idx){
    contourWidget->setRenderMode(idx);
}

void Window::renderHollowSlot(){
    bool state = ui->renderHollowBox->checkState();
    contourWidget->setRenderHollow(state);
}
