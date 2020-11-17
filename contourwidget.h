#ifndef CONTOURWIDGET_H
#define CONTOURWIDGET_H
#include "openglwidget.h"


class ContourWidget : public OpenGLWidget{
    Q_OBJECT
public:
    ContourWidget(){
        //To store inital config space
        renderMode = 0;
        renderHollow = false;
        m_pGameCamera = NULL;
        m_pEffect = NULL;
        m_scale = 3.1f;
        s_default = NULL;
        //Lighting settings
        //mauve m_directionalLight.Color = glm::vec3(.74f, .57f, .63f);
        m_directionalLight.Color = QVector3D(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 1.0f;
        m_directionalLight.DiffuseIntensity = .50f;
        m_directionalLight.Direction = QVector3D(1.0f, -1.0f, 0.0f);

        m_directionalLightDark.Color = QVector3D(0.0f, 0.0f, 0.0f);
        m_directionalLightDark.AmbientIntensity = 0.9f;
        m_directionalLightDark.DiffuseIntensity = .50f;
        m_directionalLightDark.Direction = QVector3D(1.0f, -1.0f, 0.0f);


        //Camera/Window initializing settings
        m_persProjInfo.FOV = 50.0f;
        m_persProjInfo.zNear = 0.1f;
        m_persProjInfo.zFar = 100.0f;
    }
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int x, int y) override;
void setModelFilePath(std::string filepath);
void setRenderMode(int choice);
void setRenderHollow(bool state);

private:
    int renderMode;
    bool renderHollow;
    virtual void mouseMoveEvent(QMouseEvent *ev) override;
    virtual void mousePressEvent(QMouseEvent * ev) override;
    virtual void mouseReleaseEvent(QMouseEvent * ev) override;
    bool clicked;
    float m_scale = 1;
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    GLuint gWVPLocation =0;
    GLuint gSampler =0;
    LightingTechnique* m_pEffect;
    Mesh* m_pMesh;
    Camera* m_pGameCamera;
    Pipeline p;
    DirectionalLight m_directionalLight;
    DirectionalLight m_directionalLightDark;

    PersProjInfo m_persProjInfo;
    LightingTechnique* s_default;
    int m_frame = 0;
    std::string m_modelFilePath = "../../Models/bunny/reconstruction/bun_zipper_res2.ply";
    std::string defaultVertShader = "../shaders/SH_default.vert";
    std::string defaultFragShader = "../shaders/SH_default.frag";

};



#endif // CONTOURWIDGET_H
