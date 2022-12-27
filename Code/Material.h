#pragma once

#include "GL/glew.h"
class Material
{
public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shinePower);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shinePowerLocation) const;

private:
	GLfloat specularIntensity;
	GLfloat shinePower;
};

