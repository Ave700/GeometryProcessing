
#include "contourwidget.h"
#include "window.h"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QSlider>

Window::Window(MainWindow *mw) : mainWindow(mw){
    contourWidget = new ContourWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    scaleSlider = createSlider();
    connect(scaleSlider, &QSlider::valueChanged, contourWidget, &ContourWidget::setScale);
    connect(contourWidget, &ContourWidget::scaleChanged, scaleSlider, &QSlider::setValue);

    container->addWidget(contourWidget);
    container->addWidget(scaleSlider);
    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);

    setLayout(mainLayout);

    setWindowTitle(tr("Hello"));
}


void Window::setContourFilePath(std::string filepath){
    contourWidget->setModelFilePath(filepath);
}

void Window::setContourScale(double scale){
    contourWidget->setScale(scale);
}

QSlider *Window::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setSliderPosition(310);
    slider->setRange(0, 500);
    slider->setSingleStep(1);
    slider->setPageStep(1 * 1);
    slider->setTickInterval(50 * 1);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}
