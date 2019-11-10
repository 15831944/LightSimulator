#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include <OpenGL/glad/include/glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "resourceManager/texture/texture.h"
#include "renderers/spriteRenderer/SpriteRenderer.h"
#include "renderers/rayRenderer/RayRenderer.h"
#include "objectManager/object/object.h"
#include "objectManager/object/objectTemplate.h"
#include "resourceManager/resourceManager.h"

//lightObject class inherits from Object class.
class lightObject : public ObjectTemplate {
private:
	bool hasRotated;
public:
	glm::vec2 rayDirection; //unit vector
	glm::vec2 rayOrigin, rayEnd; //(x1, y1)
	glm::vec3 rayColour;

	ObjectTemplate *directionIndicator = nullptr;

	float currentRefractiveIndex = 1.0f;

	lightObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, bool fixed = false);
	void Draw(SpriteRenderer &renderer);
	void DrawRay(RayRenderer &renderer, glm::vec2 v0, glm::vec2 v1);
	void moveObject(glm::vec2 newPos);
	void rotateObject(float angle);
	float calculateAngle(glm::vec2 rayDir, glm::vec2 normal);
	void rotateToMouse(glm::vec3 coords);
	void rotateToIndicator();
	void rotateByAngle(float angle);
};

#endif