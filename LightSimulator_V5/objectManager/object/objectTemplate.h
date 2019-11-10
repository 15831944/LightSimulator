#ifndef OBJECTTEMPLATE_H
#define OBJECTTEMPLATE_H

#include <OpenGL/glad/include/glad/glad.h>
#include <glm/glm.hpp>

#include "resourceManager/texture/texture.h"
#include "renderers/spriteRenderer/SpriteRenderer.h"


// Container object for holding all states relevant for a single object entity.
class ObjectTemplate
{
public:
	// Object state
	glm::vec3   Position, Size, Color;
	GLfloat     Rotation;
	Texture2D   Sprite;
	bool		FixedPosition;
	// Constructor(s)
	ObjectTemplate();
	ObjectTemplate(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f), bool fixed = false);
	// Draw sprite
	virtual void Draw(SpriteRenderer &renderer);
	virtual void moveObject(glm::vec2 newPos);
	virtual void rotateObject(float angle);

	//Used for mouse picking
	bool CalculateRayCollision3D(glm::vec3 &rOrigin, glm::vec3 &rDir, glm::vec3& intersectionPoint, float& lowestFraction);
	bool isRayInsideObject(glm::vec2 &rOrigin);

private:
	bool clipLine(int dimension, const glm::vec3& origin, const glm::vec3& end, float& f_low, float& f_high);

};

#endif