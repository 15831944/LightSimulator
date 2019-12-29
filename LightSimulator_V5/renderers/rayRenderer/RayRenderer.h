#ifndef RAYRENDERER_H
#define RAYRENDERER_H

#include <OpenGL/glad/include/glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader/shader.h"


class RayRenderer
{
public:
	// Constructor
	RayRenderer(Shader &shader);
	// Destructor
	~RayRenderer();

	void Draw(glm::vec2 origin, glm::vec2 end, glm::vec3 colour, float transparency);
private:
	Shader rayShader;
	GLuint rayVAO, rayVBO;
	// Initializes and configures the buffer and vertex attributes
	void initRenderData();
};

#endif