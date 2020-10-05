#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include <QWindow>
#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

//! [1]
class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow* parent = nullptr);
    ~OpenGLWindow();

    virtual void render(QPainter* painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent* event) override;
    virtual void resizeGL(int w, int h);
    virtual void mouseMoveEvent(QMouseEvent* ev) override;
    virtual void mousePressEvent(QMouseEvent* ev) override;
    void exposeEvent(QExposeEvent* event) override;

private:
    bool m_animating = false;

    QOpenGLContext* m_context = nullptr;
    QOpenGLPaintDevice* m_device = nullptr;
};
//! [1]


#endif // OPENGLWINDOW_H
