#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getKeys() { return keys; }
	
	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
	
	bool keys[1024];
	
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks() const;
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

