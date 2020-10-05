#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
QT_BEGIN_NAMESPACE
class ContourWidget;
class OpenGLWidget;
class MainWindow;
class QSlider;
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT
public:
    Window(MainWindow *mw);
    void setContourFilePath(std::string filepath);
    void setContourScale(double scale);

private:
    QSlider *createSlider();

    QSlider *scaleSlider;
    OpenGLWidget *openGLWidget;
    ContourWidget* contourWidget;
    MainWindow *mainWindow;
};

#endif // WINDOW_H
