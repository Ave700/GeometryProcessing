#pragma once
#ifndef TECHNIQUE_H
#define TECHNIQUE_H
#include "globals.h"
#include <list>

#include "utilities.h"
//Parent class to contain shader techniques for example lighting or otherwise
class Technique
{
public:
	Technique();

	virtual ~Technique();

	virtual bool Init();

	//wrapper function for glUseProgram() must be called whenever switching technique
	void Enable();

protected:
	bool AddShader(GLenum ShaderType, const char* pShaderText);

	bool Finalize();

	GLint GetUniformLocation(const char* pUniformName);
private:
	GLuint m_shaderProg;

	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};
#endif
