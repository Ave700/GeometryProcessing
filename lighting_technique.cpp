#include "lighting_technique.h"
LightingTechnique::LightingTechnique()
{
}

bool LightingTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}
	
	if (!AddShader(GL_VERTEX_SHADER, "lighting.vs")) {
		return false;
	}
	if (!AddShader(GL_FRAGMENT_SHADER, "lighting.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	//Finding all uniforms and checking they're at valid location
	m_WVPLocation = GetUniformLocation("gWVP");
	m_samplerLocaiton = GetUniformLocation("gSampler");
	m_WorldMatrixLocation = GetUniformLocation("gWorld");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");

	//Loop to set and check uniforms for all point lights
	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		//color 
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = GetUniformLocation(Name);
		
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

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
		m_samplerLocaiton == INVALID_UNIFORM_LOCATION					 ||
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
void LightingTechnique::SetTextureUnit(unsigned int TextureUnit) {
	glUniform1i(m_samplerLocaiton, TextureUnit);
}


void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x(), Light.Color.y(), Light.Color.z());
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	QVector3D Direction = Light.Direction;
	Direction.normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x(), Direction.y(), Direction.z());
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightingTechnique::SetEyeWorldPos(const QVector3D& EyeWorldPos) {
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x(), EyeWorldPos.y(), EyeWorldPos.z());
}

void LightingTechnique::SetMatSpecularIntensity(float Intensity) {
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void LightingTechnique::SetMatSpecularPower(float Power) {
	glUniform1f(m_matSpecularPowerLocation, Power);
}

void LightingTechnique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(m_numPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		//General Light Sets
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x(), pLights[i].Color.y(), pLights[i].Color.z());
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x(), pLights[i].Position.y(), pLights[i].Position.z());
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		//Atten sets
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}