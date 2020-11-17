#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "globals.h"
#include <QMainWindow>
#include <QList>

#include "window.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenu *fileMenu;
    QAction *openAct;
    Window *glWindow;

    void createActions();
    void createMenus();

private slots:

void open();


#ifndef QT_NO_CONTEXTMENU
void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
};
#endif // MAINWINDOW_H
