#include "OpenGL/imgui/imgui.h"
#include "OpenGL/imgui/imgui_impl_glfw.h"
#include "OpenGL/imgui/imgui_impl_opengl3.h"

#include <OpenGL/glad/include/glad/glad.h>
#include <OpenGL/glfw-3.2.1/include/glfw3.h>
#include <iostream>
#include "program/program.h"
#include "resourceManager/resourceManager.h"

//TODO: PREVENT OVERLAPPING OBJECTS WHEN MOVING THEM (USE HIT DETECTION).

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// The Width of the screen
const GLuint SCREEN_WIDTH = 1200;
// The height of the screen
const GLuint SCREEN_HEIGHT = 1000;

Program Simulator(SCREEN_WIDTH, SCREEN_HEIGHT);
bool firstMouse = true;
float lastX = 1200.0f / 2.0f;
float lastY = 1000.0f / 2.0f;

int main(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Light Simulator", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);//Vsync

	
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//allows for rendering transparent textures.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize game
	Simulator.Init();

	const char* glsl_version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Start Game within Menu State
	Simulator.State = PROGRAM_ACTIVE;

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// Manage user input
		Simulator.ProcessInput(deltaTime);

		// Update Game state
		Simulator.Update(deltaTime);

		// Render
		glClearColor(Simulator.backgroundColour.r, Simulator.backgroundColour.g, Simulator.backgroundColour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Simulator.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources loaded using the resource manager
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Simulator.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Simulator.Keys[key] = GL_FALSE;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT || GLFW_MOUSE_BUTTON_MIDDLE) && action == GLFW_PRESS)
	{
		//getting cursor position when left mouse button is pressed.
		glfwGetCursorPos(window, &Simulator.mouseX, &Simulator.mouseY);
		Simulator.mouse[button] = GL_TRUE;
	}
	else if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT || GLFW_MOUSE_BUTTON_MIDDLE) && action == GLFW_RELEASE) {
		Simulator.mouse[button] = GL_FALSE;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Simulator.mouseX = xpos;
	Simulator.mouseY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) //used for zooming in and out.
{
	if (yoffset > 0) {
		//scrolled up
		std::cout << "UP" << std::endl;
	}
	else if(yoffset < 0){
		//scrolled down
		std::cout << "Down" << std::endl;
	}
}
