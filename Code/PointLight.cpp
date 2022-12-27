#include "PointLight.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "OmniShadowMap.h"

PointLight::PointLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    exponent = 0.0;
    linear = 0.0f;
    constant = 1.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientColourIntensity, GLfloat diffuseIntensity,
    GLfloat xPosition, GLfloat yPosition, GLfloat zPosition,
    GLfloat exponent, GLfloat linear, GLfloat constant,
    GLint shadowWidth, GLint shadowHaight,
    GLfloat near, GLfloat far) : Light(red, green, blue, ambientColourIntensity, diffuseIntensity,
        shadowWidth, shadowHaight)
{
    position = glm::vec3(xPosition, yPosition, zPosition);
    this->exponent = exponent;
    this->linear = linear;
    this->constant = constant;

    farPlane = far;

    float aspectRatio = (float) shadowWidth / (float) shadowHaight;
    lightProj = glm::perspective(glm::radians(90.0f), aspectRatio, near, far);

    shadowMap = new OmniShadowMap();
    shadowMap->Init(shadowWidth, shadowHaight);
}

void PointLight::UseLight(GLuint colourLocation, GLuint ambientIntensityLocation,
    GLuint diffuseIntensityLocation, GLuint positionLocation,
    GLuint exponentLocation, GLuint linearLocation, GLuint constantLocation)
{
    glUniform3f(colourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(exponentLocation, exponent);
    glUniform1f(linearLocation, linear);
    glUniform1f(constantLocation, constant);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
    std::vector<glm::mat4> lightMatrices;
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    return lightMatrices;
}

PointLight::~PointLight(){}