#pragma once
#include "GL/glew.h"

class ShadowMap
{
public:
    ShadowMap();
    virtual ~ShadowMap();
    
    virtual bool Init(GLuint width, GLuint height);
    virtual void Write();
    virtual void Read(GLenum textureUnit);

    GLuint GetShadowWidth() {return shadowWidth;}
    GLuint GetShadowHeight() {return shadowHeight;}
    
protected:
    GLuint FBO, shadowMap;
    GLuint shadowWidth, shadowHeight;
};
