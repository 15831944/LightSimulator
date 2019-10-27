#include "program.h"
#include <vector>
#include <iostream>

GLfloat Velocity = 500.0f;

Program::Program(GLuint width, GLuint height)
	: State(PROGRAM_ACTIVE), Keys(), Width(width), Height(height)
{

}

Program::~Program()
{
	//delete Renderer;
}

void Program::Init()
{
	//Set background colour.
	backgroundColour = glm::vec3(0.10f, 0.10f, 0.10f);
	//Create orthographic projection matrix.
	projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	// Load shaders
	ResourceManager::LoadShader("shader/shaderFiles/vShader.txt", "shader/shaderFiles/fShader.txt", nullptr, "sprite");
	ResourceManager::LoadShader("shader/shaderFiles/rvShader.txt", "shader/shaderFiles/rfShader.txt", nullptr, "lightRay");
	// Configure shaders
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	ResourceManager::GetShader("lightRay").Use().SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("block_solid.png", GL_TRUE, "sblock");
	ResourceManager::LoadTexture("block.png", GL_TRUE, "block");
	ResourceManager::LoadTexture("angleIndicatorIcon.png", GL_TRUE, "angleIndicator");
	// Set render-specific controls
	Shader quadShader;
	Shader rayShader;
	quadShader = ResourceManager::GetShader("sprite");
	rayShader = ResourceManager::GetShader("lightRay");
	//create object manager
	oManager = new objectManager(quadShader, rayShader);
	//create default objects
	oManager->addObject(glm::vec2(900, 550), glm::vec2(200, 200), "sblock");
	oManager->addLight(glm::vec2(500, 570), glm::vec2(40, 40), "block");
}

void Program::Update(GLfloat dt)
{
	//oManager->selectObject(glm::vec3((float)mouseX, (float)mouseY, 1.0f), false, this->mouse[GLFW_MOUSE_BUTTON_1], Keys[GLFW_KEY_E], Keys[GLFW_KEY_R]);
}


void Program::ProcessInput(GLfloat dt)
{
	if(this->State == PROGRAM_ACTIVE) {
		//select and deselect objects.
		/*if(this->mouse[GLFW_MOUSE_BUTTON_1]) {
			oManager->selectObject(glm::vec3((float)mouseX, (float)mouseY, 1.0f), true, this->mouse[GLFW_MOUSE_BUTTON_1], Keys[GLFW_KEY_E], Keys[GLFW_KEY_R]);
		}*/

		oManager->selectObject(glm::vec3((float)mouseX, (float)mouseY, 1.0f), this->mouse[GLFW_MOUSE_BUTTON_1], this->mouse[GLFW_MOUSE_BUTTON_2]);

		//Dynamically adding objects
		if (this->mouse[GLFW_MOUSE_BUTTON_3]) {
			oManager->addObject(glm::vec2((float)mouseX, (float)mouseY), glm::vec2(100, 100), "sblock");
			this->mouse[GLFW_MOUSE_BUTTON_3] = 0;
		}
		/*if (this->mouse[GLFW_MOUSE_BUTTON_2]) {
			oManager->addLight(glm::vec2((float)mouseX, (float)mouseY), glm::vec2(40, 40), "block");
		}*/
	}
}

void Program::Render()
{
	oManager->drawAll();
}
