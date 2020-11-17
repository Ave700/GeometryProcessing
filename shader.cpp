#include "shader.h"

Shader::Shader(){
}

void Shader::init(){
    if(initialized){
        qCritical("This shader was already initialized!");
        return;
    }

}

void Shader::initPretty(){
    if(initialized){
        qCritical("This shader was already initialized!");
        return;
    }

}

void Shader::use(){
    thisProgram.bind();
}
