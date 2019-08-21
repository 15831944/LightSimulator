#ifndef OBJECT_H
#define OBJECT_H

#include <OpenGL/glad/include/glad/glad.h>
#include <glm/glm.hpp>

#include "resourceManager/texture/texture.h"
#include "renderers/spriteRenderer/SpriteRenderer.h"


// Container object for holding all states relevant for a single object entity.
class Object
{
public:
	// Object state
	glm::vec3   Position, Size, Color;
	GLfloat     Rotation;
	float		refractiveIndex;
	Texture2D   Sprite;
	// Constructor(s)
	Object();
	Object(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f));
	// Draw sprite
	virtual void Draw(SpriteRenderer &renderer);
	virtual void moveObject(glm::vec2 newPos);
	virtual void rotateObject(float angle);
	bool CalculateRayCollision3D(glm::vec3 &rOrigin, glm::vec3 &rDir, glm::vec3& intersectionPoint, float& lowestFraction);
	bool CalculateRayCollision(glm::vec2 &rOrigin, glm::vec2 &rDir, glm::vec2& intersectionPoint, float& lowestFraction);
	bool isRayInsideObject(glm::vec2 &rOrigin);
private:
	bool clipLine(int dimension, const glm::vec3& origin, const glm::vec3& end, float& f_low, float& f_high);

};

#endif