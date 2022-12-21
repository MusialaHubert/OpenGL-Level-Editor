#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();
	
	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numberOfVertices, unsigned int numberOfIndices);
	void CreateBasicTriangle();
	void CreateBasicPlane();
	void RenderMesh() const;
	void CalculateAverageNormals(unsigned int* indicies, unsigned int indiceCount, GLfloat* vertices,
		unsigned int verticeCount, unsigned int vLenght, unsigned int normalOffset);

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

	void ClearMesh();
};

