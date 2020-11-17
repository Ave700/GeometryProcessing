#ifndef SHADER_H
#define SHADER_H
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

class Shader
{
public:
    unsigned int ID;

    //Constructor to read and build shader
    Shader(const char* vertexPath, const char* fragmentPath);

    void use();
    void init();
    void initPretty();
    QOpenGLShaderProgram thisProgram;

private:
    bool initialized;


};

#endif // SHADER_H
