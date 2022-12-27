#pragma once

#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);
	~Texture();
	
	bool LoadTexture(bool withAlpha);
	void UseTexture() const;
	
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;

	void ClearTexture();
};

