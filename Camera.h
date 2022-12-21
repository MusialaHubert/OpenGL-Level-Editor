#pragma once 

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed);

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 GetCameraPosition() const { return position; }
	glm::vec3 GetCameraDirection() const { return normalize(front); }
	glm::mat4 CalculateViewMatrix() const;

	GLfloat GetYaw() {return yaw;}
	GLfloat GetPitch() {return pitch;}

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};

