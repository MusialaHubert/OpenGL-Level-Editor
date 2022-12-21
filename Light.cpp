#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		GLint shadowWidth, GLint shadowHaight)
{
	colour = glm::vec3(red, green, blue);
	this->ambientIntensity = ambientIntensity;
	this->diffuseIntensity = diffuseIntensity;

	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHaight);
}

