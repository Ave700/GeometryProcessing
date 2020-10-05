#include "globals.h"
#include "mainwindow.h"
#include "window.h"
#include "openglwinget.h"
#include <QApplication>
#include <QLabel>
#include <QOpenGLWidget>
#include "openglwidget.h"



int main(int argc, char *argv[])
{
    Magick::InitializeMagick(*argv);
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);


    MainWindow w;

    w.show();



    return app.exec();
}
