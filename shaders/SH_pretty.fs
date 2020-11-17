#version 330

//Must be same as in lighting technique header
const int MAX_POINT_LIGHTS = 2;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct BaseLight{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	BaseLight Base;
	vec3 Direction;
};

struct Attenuation{
	float Constant;
	float Linear;
	float Exp;
};

struct PointLight{
	BaseLight Base;
	vec3 Position;
	Attenuation Atten;
};

uniform int gNumPointLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform sampler2D gSampler;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;


//There are shared light calculations between types of lights. These are encapsulated here.
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)
{
	vec4 AmbientColor = vec4(Light.Color* Light.AmbientIntensity, 1.0f);
	float DiffuseFactor = dot(Normal, -LightDirection);

	//if angle of reflection is too small spec and diff will have no effect
	vec4 DiffuseColor = vec4(0,0,0,0);
	vec4 SpecularColor = vec4(0,0,0,0);

	if(DiffuseFactor > 0){
		DiffuseColor = vec4(Light.Color* Light.DiffuseIntensity * DiffuseFactor, 1.0f) ;

		vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);
		if(SpecularFactor > 0) {
			SpecularFactor = pow(SpecularFactor,gSpecularPower);
			SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0f);
		}
	}
	return (AmbientColor + DiffuseColor + SpecularColor);
}

//Directional light calc
vec4 CalcDirectionalLight(vec3 Normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);
}

vec4 CalcPointLight(int Index, vec3 Normal)
{
	vec3 LightDirection = WorldPos0 - gPointLights[Index].Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = CalcLightInternal(gPointLights[Index].Base, LightDirection, Normal);
	float Attenuation = gPointLights[Index].Atten.Constant +
						gPointLights[Index].Atten.Linear * Distance +
						gPointLights[Index].Atten.Exp * Distance * Distance;

	return Color / Attenuation;
}

void main()
{
	
	//Light direction is from vertex point of view, therefor the light is in the opposite direction from where it points
	vec3 Normal = normalize(Normal0);

	vec4 TotalLight = CalcDirectionalLight(Normal);

	for(int i = 0 ; i < gNumPointLights ; i++){
		TotalLight += CalcPointLight(i, Normal);
	}
	vec4 color = texture2D(gSampler, TexCoord0.xy);
	
	if(color.x < 1.0f){
		FragColor = color * TotalLight* vec4(0f,0f,0f,1f);
	}
	else{
		FragColor = color * TotalLight;
	}
	
	//FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight* vec4(0,0,0,0);
}