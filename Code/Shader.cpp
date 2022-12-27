#include "Shader.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp> 

Shader::Shader()
{
	ClearShader();
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	const std::string vertexString = ReadFile(vertexLocation);
	const std::string geometryString = ReadFile(geometryLocation);
	const std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::Validate() const
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

std::string Shader::ReadFile(const char* fileLocation) const
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read, %s!, File do not exist", fileLocation);
		return "";
	}

	std::string line;
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}


void Shader::SetDirectionalLight(DirectionalLight* directionalLight) const
{
	directionalLight->UseLght(uniformDirectionalLight.uniformColour, uniformDirectionalLight.uniformAmbientIntensity,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pointLight, unsigned lightCount, unsigned int startTextureUnit, unsigned int offset) const
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pointLight[i].UseLight(uniformPointLight[i].uniformColour, uniformPointLight[i].uniformAmbientIntensity,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformExponent, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformConstant);

		pointLight[i].GetShadowMap()->Read(GL_TEXTURE0 + startTextureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, startTextureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, pointLight[i].GetFarPlane());
	}
}

void Shader::SetSpotLights(SpotLight* spotLight, unsigned lightCount, unsigned int startTextureUnit, unsigned int offset) const
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		spotLight[i].UseLight(uniformSpotLight[i].uniformColour, uniformSpotLight[i].uniformAmbientIntensity,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformEdge);

		spotLight[i].GetShadowMap()->Read(GL_TEXTURE0 + startTextureUnit + i);
		
		glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, startTextureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, spotLight[i].GetFarPlane());
	}
}

void Shader::SetTexture(GLuint textureUnit) const
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit) const
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* transform) const
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*transform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices) const
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(uniformlightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::UseShader() const
{
	glUseProgram(shaderID);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
	}

	pointLightCount = 0;
	spotLightCount = 0;
	
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	uniformCameraPosition = 0;
	uniformSpecularIntensity = 0;
	uniformShinePower = 0;
	uniformPointLightCount = 0;
	uniformSpotLightCount = 0;
	uniformTexture = 0;
	uniformDirectionalLightTransform = 0;
	uniformDirectionalShadowMap = 0;
	uniformOmniLightPos = 0;
	uniformFarPlane = 0;
	uniformDirectionalLight.uniformColour = 0;
	uniformDirectionalLight.uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformDiffuseIntensity = 0;
	uniformDirectionalLight.uniformDirection = 0;
}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}
	
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformCameraPosition = glGetUniformLocation(shaderID, "cameraPosition");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShinePower = glGetUniformLocation(shaderID, "material.shinePower");
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuffer);
	}

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, localBuffer);
	}
	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for(size_t i = 0; i < 6; i++)
	{
		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "lightMatrices[%d]", i);
		uniformlightMatrices[i] = glGetUniformLocation(shaderID, localBuffer);
	}

	for(size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMaps[i].shadowMap = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMaps[i].farPlane = glGetUniformLocation(shaderID, localBuffer);
	}
}
