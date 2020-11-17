#include "lighting_technique.h"
LightingTechnique::LightingTechnique(const char* vertexPath, const char* fragmentPath)
{
    Init(vertexPath, fragmentPath);
}

bool LightingTechnique::Init(const char* vertexPath, const char* fragmentPath)
{
    QOpenGLShader s_Vertex(QOpenGLShader::Vertex);
    QOpenGLShader s_Fragment(QOpenGLShader::Fragment);

    s_Vertex.compileSourceFile(vertexPath);
    s_Fragment.compileSourceFile(fragmentPath);

    thisProgram.addShader(&s_Vertex);
    thisProgram.addShader(&s_Fragment);
    thisProgram.link();
	//Finding all uniforms and checking they're at valid location
    m_posAttr = thisProgram.attributeLocation("posAttr");
    m_normAttr = thisProgram.attributeLocation("normAttr");
    m_colAttr = thisProgram.uniformLocation("colAttr");
    m_WVPLocation = thisProgram.uniformLocation("gWVP");
    m_WorldMatrixLocation = thisProgram.uniformLocation("gWorld");
    m_eyeWorldPosLocation = thisProgram.uniformLocation("gEyeWorldPos");
    m_dirLightLocation.Color = thisProgram.uniformLocation("gDirectionalLight.Base.Color");
    m_dirLightLocation.AmbientIntensity = thisProgram.uniformLocation("gDirectionalLight.Base.AmbientIntensity");
    m_dirLightLocation.Direction = thisProgram.uniformLocation("gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = thisProgram.uniformLocation("gDirectionalLight.Base.DiffuseIntensity");
    m_matSpecularIntensityLocation = thisProgram.uniformLocation("gMatSpecularIntensity");
    m_matSpecularPowerLocation = thisProgram.uniformLocation("gSpecularPower");
    m_numPointLightsLocation = thisProgram.uniformLocation("gNumPointLights");

	//Loop to set and check uniforms for all point lights
	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		//color 
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
        m_pointLightsLocation[i].Color = thisProgram.uniformLocation(Name);
		
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        m_pointLightsLocation[i].AmbientIntensity = thisProgram.uniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        m_pointLightsLocation[i].DiffuseIntensity = thisProgram.uniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
        m_pointLightsLocation[i].Position = thisProgram.uniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        m_pointLightsLocation[i].Atten.Constant = thisProgram.uniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        m_pointLightsLocation[i].Atten.Linear = thisProgram.uniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        m_pointLightsLocation[i].Atten.Exp = thisProgram.uniformLocation(Name);

		if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION			  ||
			m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION		  ||
			m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION	  ||
			m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION	  ||
			m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	//Making sure all locations aren't blank, if they are an error occured accessing location
	if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION  ||
		m_WVPLocation == INVALID_UNIFORM_LOCATION						 ||
		m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION				 ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION			 ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION  ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION		 ||
		m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION		 ||
		m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION			 ||
		m_numPointLightsLocation == INVALID_UNIFORM_LOCATION )
	{
		return false;
	}
	return true;
}

//TODO Refactor to work with QT
//void LightingTechnique::SetWVP(const QMatrix4x4& WVP) {
//	glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, (const GLfloat*)WVP); // MAy break something not sure
//}

//void LightingTechnique::SetWorldMatrix(const QMatrix4x4& WorldInverse) {
//	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, WorldInverse);
//}
//void LightingTechnique::SetTextureUnit(unsigned int TextureUnit) {
//    thisProgram.setUniformValue(m_samplerLocaiton, TextureUnit);
//}


void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
    thisProgram.setUniformValue(m_dirLightLocation.Color, Light.Color);
    thisProgram.setUniformValue(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	QVector3D Direction = Light.Direction;
	Direction.normalize();
    thisProgram.setUniformValue(m_dirLightLocation.Direction, Direction);
    thisProgram.setUniformValue(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightingTechnique::SetEyeWorldPos(const QVector3D& EyeWorldPos) {
    thisProgram.setUniformValue(m_eyeWorldPosLocation, EyeWorldPos);
}

void LightingTechnique::SetMatSpecularIntensity(float Intensity) {
    thisProgram.setUniformValue(m_matSpecularIntensityLocation, Intensity);
}

void LightingTechnique::SetMatSpecularPower(float Power) {
    thisProgram.setUniformValue(m_matSpecularPowerLocation, Power);
}

void LightingTechnique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
    thisProgram.setUniformValue(m_numPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		//General Light Sets
        thisProgram.setUniformValue(m_pointLightsLocation[i].Color, pLights[i].Color);
        thisProgram.setUniformValue(m_pointLightsLocation[i].Position, pLights[i].Position);
        thisProgram.setUniformValue(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
        thisProgram.setUniformValue(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		//Atten sets
        thisProgram.setUniformValue(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
        thisProgram.setUniformValue(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
        thisProgram.setUniformValue(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}
