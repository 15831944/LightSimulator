#pragma once
#include <OpenGL/glad/include/glad/glad.h>
#include <OpenGL/glfw-3.2.1/include/glfw3.h>
#include "objectManager/objectManager.h"
#include "resourceManager/resourceManager.h"

enum programState {
	PROGRAM_ACTIVE,
	PROGRAM_MENU
};

class Program {
private:
	objectManager* oManager;
public:
	programState State;
	bool Keys[1024];
	bool mouse[3];
	double mouseX, mouseY;
	unsigned int Width, Height;
	glm::vec3 backgroundColour;
	glm::mat4 projection;

	Program(GLuint width, GLuint height);
	~Program();

	void Init();

	//program loop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};