#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		GLint shadowWidth, GLint shadowHaight);

	ShadowMap* GetShadowMap() { return shadowMap; }

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

