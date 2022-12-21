#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat colourIntensity, GLfloat diffuseIntensity,
        GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,
        GLint shadowWidth, GLint shadowHaight);

    void UseLght(GLuint colourLocation, GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation, GLuint directionLocation);

    glm::mat4 CalculateLightTransform();

private:
    glm::vec3 direction;
};
