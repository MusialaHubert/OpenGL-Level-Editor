#pragma once

#include <string>
#include <fstream>

#include <GL/glew.h>

#include "Constants.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();
	~Shader();
	
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	
	void Validate() const;

	std::string ReadFile(const char* fileLocation) const;

	GLuint GetProjectionLocation() const { return uniformProjection; }
	GLuint GetModelLocation() const { return uniformModel; }
	GLuint GetViewLocation() const { return uniformView; }
	GLuint GetCameraPositionLocation() const { return uniformCameraPosition; }
	GLuint GetSpecularIntensityLocation() const { return uniformSpecularIntensity; }
	GLuint GetShinePower() const { return uniformShinePower; }
	GLuint GetColourLocation() const { return uniformDirectionalLight.uniformColour; }
	GLuint GetAmbientIntensityLocation() const { return uniformDirectionalLight.uniformAmbientIntensity; }
	GLuint GetDiffuseIntensityLocation() const { return uniformDirectionalLight.uniformDiffuseIntensity; }
	GLuint GetDirectionLocation() const { return uniformDirectionalLight.uniformDirection; }
	GLuint GetOmniLightPositionLocation() const { return uniformOmniLightPos; }
	GLuint GetFarPlaneLocation() const { return uniformFarPlane; }

	void SetDirectionalLight(DirectionalLight* directionalLight) const;
	void SetPointLights(PointLight* pointLight, unsigned int lightCount, unsigned int startTextureUnit, unsigned int offset) const;
	void SetSpotLights(SpotLight* spotLight, unsigned int lightCount, unsigned int startTextureUnit, unsigned int offset) const;
	void SetTexture(GLuint textureUnit) const;
	void SetDirectionalShadowMap(GLuint textureUnit) const;
	void SetDirectionalLightTransform(glm::mat4* transform) const;
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices) const;

	void UseShader() const;
	
private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformCameraPosition;
	GLuint uniformSpecularIntensity, uniformShinePower;
	GLuint uniformPointLightCount;
	GLuint uniformSpotLightCount;
	GLuint uniformTexture, uniformDirectionalLightTransform, uniformDirectionalShadowMap;
	GLuint uniformOmniLightPos, uniformFarPlane;

	GLuint uniformlightMatrices[6];

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformExponent;
		GLuint uniformLinear;
		GLuint uniformConstant;
	} uniformPointLight[MAX_POINT_LIGHTS];

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformDirection;
		GLuint uniformExponent;
		GLuint uniformLinear;
		GLuint uniformConstant;

		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	struct
	{
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void ClearShader();

	void CompileProgram();
};

