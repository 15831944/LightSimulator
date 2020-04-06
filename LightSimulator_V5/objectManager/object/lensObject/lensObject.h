#ifndef LENSOBJECT_H
#define LENSOBJECT_H

#include "objectManager/object/objectTemplate.h"

// Container object for holding all states relevant for a single object entity.
class lensObject : public ObjectTemplate
{
public:
	enum materialType {
		concave,
		convex
	}material;
	// Constructor(s)
	lensObject();
	lensObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), bool fixed = false);

	glm::vec2 calculateFocalPoint();

	//ray to AABB collision detection
	bool CalculateRayCollision(glm::vec2 &rOrigin, glm::vec2 &rDir, glm::vec2& intersectionPoint, float& lowestFraction);
	bool calculateAABBcollision(lensObject &object1, lensObject &object2);
};

#endif