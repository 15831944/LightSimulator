#include "RayRenderer.h"

RayRenderer::RayRenderer(Shader &shader)
{
	this->rayShader = shader;
	this->initRenderData();
}

RayRenderer::~RayRenderer() {
	glDeleteVertexArrays(1, &this->rayVAO);
}

void RayRenderer::initRenderData() {
	GLfloat vertices[] = {
		0.0f, 0.0f,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &this->rayVAO);
	glGenBuffers(1, &this->rayVBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->rayVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindVertexArray(this->rayVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void RayRenderer::Draw(glm::vec2 origin, glm::vec2 end, glm::vec3 colour) {
	GLfloat vertices[] = {
		origin.x, origin.y,
		end.x, end.y
	};
	glBindBuffer(GL_ARRAY_BUFFER, this->rayVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	this->rayShader.Use();
	this->rayShader.SetVector3f("colour", colour);
	glBindVertexArray(this->rayVAO);
	glDrawArrays(GL_LINES, 0, 2);
}