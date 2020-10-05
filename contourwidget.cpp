#include "contourwidget.h"

void ContourWidget::initializeGL()
{

    initializeOpenGLFunctions();
    qInfo("INFO: Initializing...");
    m_program = new QOpenGLShaderProgram(this);
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        //fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        qDebug("Error: '%s'", glewGetErrorString(res));
    }
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    qInfo("INFO: Linking Pos Attribute...");
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    qInfo("INFO: Linking Color Attribute...");
    m_colAttr = m_program->uniformLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    qInfo("INFO: Linking Matrix Uniform...");
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);

    qInfo("INFO: Uniform Linking Complete");
    //Modifies camera starting pos

    QVector3D Pos(0.0f, 0.0f, 0.1f);
    QVector3D Target(0.0f, 0.0f, 1.0f);
    QVector3D Up(0.0f, 1.0f, 0.0f);

    m_pGameCamera = new Camera(width(), height(), Pos, Target, Up);
    qInfo("INFO: Finished initializing Camera");
    m_pMesh = new Mesh();

    if (!m_pMesh->LoadMesh(m_modelFilePath)) {
        qCritical("CRITICAL: Failed to load mesh!");
    }

    p.SetModelCorrection((m_pMesh->getBBoxCenter(m_scale)));
    p.GetModelCorrection();
    qInfo("INFO: Initialized.");


}

void ContourWidget::paintGL()
{
    //qInfo("Rendering...");
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);



    glClearColor(1.0f, 1.0f, 1.0f,1.0f);
    m_program->bind();

    QMatrix4x4 matrix;
   /* matrix.perspective(50.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(m_pMesh->getBBoxCenter(m_scale));
    matrix.translate(0, 0, -1);

    matrix.scale(3.1);*/
    //matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    //Modifies where model is loaded
    p.Scale(m_scale);
    p.WorldPos(0.0f, 0.0f, 1.0f);

    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);
    p.GetWVPTrans();
    const QMatrix4x4& WorldTransformation = p.GetWorldTrans();
    matrix = p.GetWVPTrans();
    m_program->setUniformValue(m_matrixUniform, matrix);

   //-----------Rendering choices------------//
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//    glClearColor(0.0f,0.0f,0.0f,1.0f);
//    m_program->setUniformValue(m_colAttr, RED_OPAQUE);
//    m_pMesh->DrawContourByVertex(p.GetWVPTrans(), m_pGameCamera->GetPos());


//    m_program->setUniformValue(m_colAttr, WHITE_OPAQUE);
    m_program->setUniformValue(m_colAttr, QVector4D(.7f, .7f, .7f, 1.0f));
    m_pMesh->Render();

    //m_program->setUniformValue(m_colAttr, BLACK_OPAQUE);
   // m_pMesh->DrawContourByObjSpace(WorldTransformation, m_pGameCamera->GetPos());





    m_program->release();

    ++m_frame;
}
void ContourWidget::mouseMoveEvent(QMouseEvent *ev) {
    //event 61 is a drag and move
    if (clicked == true) {

        //trackball works in float from -width/2 to width/2 and same for y
        float x = (float)(ev->x() - width() / 2.0f) / width();
        //qt has y == 0 at top of screen should be bottom
        float y = -(float)(ev->y() - height() / 2.0f) / height();
        //qInfo("TRACKING: x:%f\ty:%f", x, y);
         p.Trackball(x,y);
   }
    update();

}
void ContourWidget::mousePressEvent(QMouseEvent* ev)  {

    clicked = true;
    float x = (float)(ev->x() - width() / 2.0f) / width();
    float y = -(float)(ev->y() - height() / 2.0f) / height();
    //qInfo("CLICKED: x:%f\ty:%f", x, y);
    p.tbInit(x, y);
     update();
}
void ContourWidget::mouseReleaseEvent(QMouseEvent* ev) {
    //qInfo("RELEASED x:%d\ty:%d", ev->x(), ev->y());
    clicked = false;
     update();
}


void ContourWidget::resizeGL(int x, int y){
    m_persProjInfo.Width = x;
    m_persProjInfo.Height = y;
}

void ContourWidget::setModelFilePath(std::string filepath){
    m_modelFilePath = filepath;
    delete m_pMesh;
    m_pMesh = new Mesh;
    if (!m_pMesh->LoadMesh(m_modelFilePath)) {
        qCritical("CRITICAL: Failed to load mesh!");
    }
update();
};

void ContourWidget::setScale(double scale){

  m_scale = scale/100.0f;
  emit scaleChanged(scale);
  p.SetModelCorrection((m_pMesh->getBBoxCenter(m_scale)));
  p.GetModelCorrection();
  update();
}
