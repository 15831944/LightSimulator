#ifndef OBJECT_H
#define OBJECT_H

#include "objectTemplate.h"

// Container object for holding all states relevant for a single object entity.
class Object : public ObjectTemplate
{
public:
	float refractiveIndex;
	enum materialType {
		opaque,
		mirror
	}material;
	// Constructor(s)
	Object();
	Object(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), bool fixed = false);
	
	//ray to AABB collision detection
	bool CalculateRayCollision(glm::vec2 &rOrigin, glm::vec2 &rDir, glm::vec2& intersectionPoint, float& lowestFraction);
	bool calculateAABBcollision(Object &object1, Object &object2);
};

#endif