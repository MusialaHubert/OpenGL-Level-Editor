#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
    SpotLight();
    SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat colourIntensity, GLfloat diffuseIntensity,
        GLfloat xPosition, GLfloat yPosition, GLfloat zPosition,
        GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
        GLfloat exponent, GLfloat linear, GLfloat constant,
        GLfloat edge,
        GLint shadowWidth, GLint shadowHaight,
        GLfloat near, GLfloat far);

    void UseLight(GLuint colourLocation, GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint exponentLocation, GLuint linearLocation, GLuint constantLocation, GLuint edgeLocation);

    void SetFlash(glm::vec3 position, glm::vec3 direction);
    void Toggle();

private:
    glm::vec3 direction;

    GLfloat edge, procEdge;

    bool isOn;
};
