#pragma once
#ifndef LIGHTING_TECHNIQUE_H
#define LIGHTING_TECHNIQUE_H

#include "3d_math_qt.h"
#include <QOpenGLShaderProgram>

//Lighting structs
struct BaseLight {
	QVector3D Color;
	float DiffuseIntensity; 
	float AmbientIntensity;

	BaseLight() {
		Color = QVector3D(0.0f, 0.0f, 0.0f);
		DiffuseIntensity = 0.0f;
		AmbientIntensity - 0.0f;
	}
};
//Like the sun
struct DirectionalLight : public BaseLight
{
	QVector3D Direction;

	DirectionalLight()
	{
		Direction = QVector3D(0.0f, 0.0f, 0.0f);
	}
};
//Like a light bulb
struct PointLight : public BaseLight
{
	QVector3D Position;

	struct {
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = QVector3D(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

class LightingTechnique
{
public: 
	static const unsigned int MAX_POINT_LIGHTS = 2;

    LightingTechnique(const char* vertexPath, const char* fragmentPath);

    virtual bool Init(const char* vertexPath, const char* fragmentPath);

	void SetWVP(const QMatrix4x4& WVP);
	void SetWorldMatrix(const QMatrix4x4& WorldInverse);
    //void SetTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(unsigned int NumLights, const PointLight* pLights);

	void SetEyeWorldPos(const QVector3D& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
    unsigned int ID;

    void use();
    void init();
    void initPretty();
    QOpenGLShaderProgram thisProgram;

private:
	//All these private variables are simply locations, these are how variables get into shaders
	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
    GLuint m_posAttr;
    GLuint m_normAttr;
    GLuint m_colAttr;
    //GLuint m_samplerLocaiton;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;

	//Location stucture for holding uniform variables
	//For directional lights
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint Direction;
		GLuint DiffuseIntensity;
	}m_dirLightLocation;

	//for point lights
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct
		{
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;

		}Atten;
	}m_pointLightsLocation[MAX_POINT_LIGHTS];
	
};
#endif
