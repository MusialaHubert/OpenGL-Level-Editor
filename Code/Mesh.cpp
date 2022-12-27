#include "Mesh.h"

#include <glm/glm.hpp> 

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numberOfVertices, unsigned int numberOfIndices)
{
	indexCount = numberOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateBasicTriangle()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);
	CreateMesh(vertices, indices, 32, 12);
}

void Mesh::CreateBasicPlane()
{
	unsigned int indices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat vertices[] = {
		-100.0f, 0.0f, -100.0f,  0.0f, 0.0f,      0.0f, -1.0f, 0.0f,
		100.0f, 0.0f, -100.0f,   100.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		-100.0f, 0.0f, 100.0f,   0.0f, 100.0f,     0.0f, -1.0f, 0.0f,
		100.0f, 0.0f, 100.0f,    100.0f, 100.0f,    0.0f, -1.0f, 0.0f,
	};
	
	CreateMesh(vertices, indices, 32, 6);
}

void Mesh::RenderMesh() const
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CalculateAverageNormals(unsigned* indicies, unsigned indiceCount, GLfloat* vertices,
	unsigned verticeCount, unsigned vLenght, unsigned normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int index0 = indicies[i] * vLenght;
		unsigned int index1 = indicies[i + 1] * vLenght;
		unsigned int index2 = indicies[i + 2] * vLenght;

		glm::vec3 vector1(vertices[index1] - vertices[index0], vertices[index1 + 1] - vertices[index0 + 1],
			vertices[index1 + 2] - vertices[index0 + 2]);

		glm::vec3 vector2(vertices[index2] - vertices[index0], vertices[index2 + 1] - vertices[index0 + 1],
			vertices[index2 + 2] - vertices[index0 + 2]);

		glm::vec3 normal = glm::cross(vector1, vector2);
		normal = glm::normalize(normal);

		index0 += normalOffset; index1 += normalOffset; index2 += normalOffset;
		vertices[index0] += normal.x; vertices[index0 + 1] += normal.y; vertices[index0 + 2] = normal.z;
		vertices[index1] += normal.x; vertices[index1 + 1] += normal.y; vertices[index1 + 2] = normal.z;
		vertices[index2] += normal.x; vertices[index2 + 1] += normal.y; vertices[index2 + 2] = normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLenght; i++)
	{
		unsigned int nOffset = i * vLenght + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x, vertices[nOffset + 1] = vec.y, vertices[nOffset + 2] = vec.z;
	}
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		IBO = 0;
	}

	indexCount = 0;
}
