#ifndef WINDOW_H
#define WINDOW_H
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QSlider>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include "optionspane.h"
QT_BEGIN_NAMESPACE
class ContourWidget;
class OpenGLWidget;
class MainWindow;
class QSlider;
namespace Ui { class ContourWidget; }
QT_END_NAMESPACE


class Window : public QWidget
{
    Q_OBJECT
public:
    Window(MainWindow *mw);
    void setContourFilePath(std::string filepath);
    void setContourScale(double scale);

private:
    Ui::ContourWidget *ui;
    QComboBox *renderOptions;
    QLabel *renderOpsLabel;
    QSlider *scaleSlider;
    OpenGLWidget *openGLWidget;
    ContourWidget* contourWidget;
    MainWindow *mainWindow;
private slots:
void renderSlot(int idx);
void renderHollowSlot();
};

#endif // WINDOW_H
