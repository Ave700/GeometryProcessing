#include "globals.h"
#include "mainwindow.h"
#include "window.h"
#include "openglwinget.h"
#include <QApplication>
#include <QLabel>
#include <QOpenGLWidget>
#include "openglwidget.h"


/*This is the main for QT, simply initializes Imagick (fornow) and calls main window.
 */
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
