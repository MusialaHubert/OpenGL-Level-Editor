#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
    direction = glm::vec3(1.0f, 0.0f, 0.0f);
    edge = 0;
    procEdge = cosf(glm::radians(edge));
    isOn = true;
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat colourIntensity, GLfloat diffuseIntensity,
    GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
    GLfloat exponent, GLfloat linear, GLfloat constant, GLfloat edge, GLint shadowWidth, GLint shadowHaight,
        GLfloat near, GLfloat far)
    : PointLight(red, green, blue, colourIntensity, diffuseIntensity,
        xPosition, yPosition, zPosition, exponent, linear, constant, shadowWidth, shadowHaight, near, far)
{
    direction = glm::normalize(glm::vec3(xDirection, yDirection, zDirection));
    this->edge = edge;
    procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
    GLuint positionLocation, GLuint directionLocation, GLuint exponentLocation, GLuint linearLocation,
    GLuint constantLocation, GLuint edgeLocation)
{
    glUniform3f(colourLocation, colour.x, colour.y, colour.z);
    if(isOn)
    {
        glUniform1f(ambientIntensityLocation, ambientIntensity);
        glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    }
    else
    {
        glUniform1f(ambientIntensityLocation, 0.0);
        glUniform1f(diffuseIntensityLocation, 0.0);
    }
    
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(exponentLocation, exponent);
    glUniform1f(linearLocation, linear);
    glUniform1f(constantLocation, constant);

    glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 position, glm::vec3 direction)
{
    this->position = position;
    this->direction = direction;
}

void SpotLight::Toggle()
{
    isOn = !isOn;
}