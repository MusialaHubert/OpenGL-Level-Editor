#include "Material.h"

Material::Material()
{
	specularIntensity = 0;
	shinePower = 0;
}

Material::Material(GLfloat specularIntensity, GLfloat shinePower)
{
	this->specularIntensity = specularIntensity;
	this->shinePower = shinePower;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shinePowerLocation) const
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shinePowerLocation, shinePower);
}
