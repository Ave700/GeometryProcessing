#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include "globals.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include <QMouseEvent>
#include "shader.h"
#include "mesh.h"
#include "pipeline.h"
#include "lighting_technique.h"
#include "colors.h"
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions{
    Q_OBJECT
public:
OpenGLWidget(QWidget *parent = nullptr);


private:
bool m_core;

};

#endif // OPENGLWIDGET_H
