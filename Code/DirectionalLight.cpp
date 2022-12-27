#include "DirectionalLight.h"

#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
    GLfloat colourIntensity, GLfloat diffuseIntensity,
    GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
    GLint shadowWidth, GLint shadowHaight) : Light(red, blue, green, colourIntensity, diffuseIntensity, shadowWidth, shadowHaight)
{
    direction = glm::vec3(xDirection, yDirection, zDirection);
    lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLght(GLuint colourLocation, GLuint ambientIntensityLocation,
    GLuint diffuseIntensityLocation, GLuint directionLocation)
{
    glUniform3f(colourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
    return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

