#pragma once
#include <vector>

#include "Light.h"

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat colourIntensity, GLfloat diffuseIntensity,
        GLfloat xPosition, GLfloat yPosition, GLfloat zPosition,
        GLfloat exponent, GLfloat linear, GLfloat constant,
        GLint shadowWidth, GLint shadowHaight,
        GLfloat near, GLfloat far);

    void UseLight(GLuint colourLocation, GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint exponentLocation, GLuint linearLocation, GLuint constantLocation);

    std::vector<glm::mat4> CalculateLightTransform();

    glm::vec3 GetPosition() { return position; }
    float GetFarPlane() {return farPlane;}
    
    ~PointLight();
protected:
    glm::vec3 position;

    GLfloat exponent, linear, constant;

    GLfloat farPlane;
};
