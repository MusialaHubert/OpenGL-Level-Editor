#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <vector>

class Shader;
class Mesh;

class SkyBox
{
public:
    SkyBox();
    SkyBox(std::vector<std::string> faceLocations);

    void DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
    Mesh* skyMesh;
    Shader* skyShader;

    GLuint textureId;
    GLuint uniformProjection, uniformView;
};
