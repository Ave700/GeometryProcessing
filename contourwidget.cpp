#include "contourwidget.h"

void ContourWidget::initializeGL()
{

    initializeOpenGLFunctions();
    qInfo("INFO: Initializing...");
    glEnable(GL_DEPTH_TEST);
    s_default = new LightingTechnique(defaultVertShader.c_str(), defaultFragShader.c_str());
    s_default->thisProgram.bind();
    //m_program = new QOpenGLShaderProgram(this);
   // m_program2 = new QOpenGLShaderProgram(this);
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        //fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        qDebug("Error: '%s'", glewGetErrorString(res));
    }

   // m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
   // m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    //m_program->link();
    //qInfo("INFO: Linking Pos Attribute...");
    //m_posAttr = s_default->thisProgram.attributeLocation("posAttr");
    //Q_ASSERT(m_posAttr != -1);
    //qInfo("INFO: Linking Color Attribute...");
    //m_colAttr = s_default->thisProgram.uniformLocation("colAttr");
    //Q_ASSERT(m_colAttr != -1);
    //qInfo("INFO: Linking Matrix Uniform...");
    //m_matrixUniform = s_default->thisProgram.uniformLocation("matrix");
    //Q_ASSERT(m_matrixUniform != -1);

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

    p.SetModelCorrection((m_pMesh->getBBoxCenter()));
    p.GetModelCorrection();
    qInfo("INFO: Initialized.");


}

void ContourWidget::paintGL()
{
    //qInfo("Rendering...");
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
   // s_default->use();
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f,1.0f);


    QMatrix4x4 matrix;

    p.Scale(m_pMesh->getScaleBestFit());
    p.WorldPos(0.0f, 0.0f, 1.0f);
    QMatrix4x4 WorldTransformation = p.GetWorldTrans();
    p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
    p.SetPerspectiveProj(m_persProjInfo);
    p.GetWVPTrans();

    matrix = p.GetWVPTrans();
    s_default->SetDirectionalLight(m_directionalLight);
    s_default->SetMatSpecularIntensity(.2f);
    s_default->SetMatSpecularPower(.5);
    s_default->SetEyeWorldPos(m_pGameCamera->GetPos());
    s_default->SetWVP(matrix);
    s_default->SetWorldMatrix(WorldTransformation);
    //setting default shaders
     s_default->use();
     //s_default->thisProgram.setUniformValue(m_matrixUniform, matrix);
   //-----------Rendering choices------------//
   // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    switch (renderMode){
    case 0:
        {

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            s_default->thisProgram.setUniformValue(m_colAttr, QVector4D(.7f, .7f, .7f, 1.0f));

            m_pMesh->Render();
            update();
            break;
        }
    case 1:
        {

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            //drawing the model the same color as background with depth test
            //hides lines behind model
            if(!renderHollow){
                s_default->thisProgram.setUniformValue(m_colAttr, WHITE_OPAQUE);
                m_pMesh->Render();
            }
            s_default->thisProgram.setUniformValue(m_colAttr, BLACK_OPAQUE);
            m_pMesh->DrawContourByObjSpace(WorldTransformation, m_pGameCamera->GetPos());
            update();
            break;
        }
    case 2:
        {

            glClearColor(0.0f,0.0f,0.0f,1.0f);
            if(!renderHollow){
                s_default->thisProgram.setUniformValue(m_colAttr, BLACK_OPAQUE);
                m_pMesh->Render();

            }

            s_default->thisProgram.setUniformValue(m_colAttr, RED_OPAQUE);
            m_pMesh->DrawContourByVertex(WorldTransformation, m_pGameCamera->GetPos());
            update();
            //m_pMesh->DrawContourByVertex(p.GetWVPTrans(), m_pGameCamera->GetPos());
            break;
        }

    case 3:
        {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            if(!renderHollow){
                s_default->thisProgram.setUniformValue(m_colAttr, WHITE_OPAQUE);
                m_pMesh->Render();
            }

            s_default->thisProgram.setUniformValue(m_colAttr, BLACK_OPAQUE);
            m_pMesh->RenderWireFrame();

            update();
        }
    }


    s_default->thisProgram.release();

    ++m_frame;
}
void ContourWidget::mouseMoveEvent(QMouseEvent *ev) {
    //event 61 is a drag and move
    if (clicked == true) {

        //trackball works in float from -width/2 to width/2 and same for y
        float x = (float)(2.0* ev->x() - width()) / width();
        float y = -(float)(2.0f *(ev->y()) - height()) / height();

         p.Trackball(x,y);
   }
    update();

}
void ContourWidget::mousePressEvent(QMouseEvent* ev)  {

    clicked = true;
    float x = (float)(2.0* ev->x() - width()) / width();
    float y = (float)(2.0f *( height() - ev->y())-height())  / height();

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

    p.Scale(m_pMesh->getScaleBestFit());
    p.SetModelCorrection((m_pMesh->getBBoxCenter()));
    p.GetModelCorrection();
update();
};

void ContourWidget::setRenderMode(int choice){
    renderMode = choice;
    update();
}

void ContourWidget::setRenderHollow(bool state){
    renderHollow = state;
    update();
}
