#include <string>
#include "technique.h"

Technique::Technique()
{
	m_shaderProg = 0;
}

//Destructor
Technique::~Technique()
{
	//Delete shader shader objects that were compiled but the object was deleted prior to linking
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
	{
		glDeleteShader(*it);
	}

	if (m_shaderProg != 0)
	{
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}
}

bool Technique::Init()
{
	m_shaderProg = glCreateProgram();

	if (m_shaderProg == 0) {
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	return true;
}

//method to add shaders to program must call finalize when done
bool Technique::AddShader(GLenum ShaderType, const char* pFilename)
{

	std::string s;
	
	if (!ReadFile(pFilename, s)) {
		return false;
	}

	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader obj of type %d\n", ShaderType);
		return false;
	}
	
	//saving shader obj to list and can be deleted by destructor
	m_shaderObjList.push_back(ShaderObj);

	const GLchar* p[1]; //to store shader text code
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint)s.size() };
	
	glShaderSource(ShaderObj, 1, p, Lengths); //both p and Lengths are of len 1 hence the 1 in the call.

	glCompileShader(ShaderObj);

	//error checking on compile
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s\n", pFilename, InfoLog);
		return false;
	}
	//finally attached compiled shader obj to program
	glAttachShader(m_shaderProg, ShaderObj);

	return true;
}


bool Technique::Finalize()
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
	}

	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return GLCheckError();
}

void Technique::Enable()
{
	glUseProgram(m_shaderProg);
}

GLint Technique::GetUniformLocation(const char* pUniformName)
{
	GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION)
	{
		fprintf(stderr, "Warning! Unable to get location of uniform '%s' \n", pUniformName);
	}

	return Location;
}

