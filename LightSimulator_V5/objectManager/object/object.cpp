#include "object.h"
#include <algorithm>
#include <math.h>
#include <iostream>

Object::Object()
	: ObjectTemplate() { }

Object::Object(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color, bool fixed)
	: ObjectTemplate(pos, size, sprite, color, fixed) {
	this->refractiveIndex = 2.417f;//diamond
	this->material = opaque;
}


bool Object::CalculateRayCollision(glm::vec2 &rOrigin, glm::vec2 &rDir, glm::vec2& intersectionPoint, float& lowestFraction) {

	glm::vec2 invdir = glm::vec2(1.0f, 1.0f) / rDir;

	float tmin, tmax, tymin, tymax;

	if (invdir.x >= 0) {
		tmin = (this->Position.x - rOrigin.x) * invdir.x;
		tmax = ((this->Position.x + this->Size.x) - rOrigin.x) * invdir.x;
	}
	else {
		tmin = ((this->Position.x + this->Size.x) - rOrigin.x) * invdir.x;
		tmax = (this->Position.x - rOrigin.x) * invdir.x;
	}

	if (invdir.y >= 0) {
		tymin = (this->Position.y - rOrigin.y) * invdir.y;
		tymax = ((this->Position.y + this->Size.y) - rOrigin.y) * invdir.y;
	}
	else {
		tymin = ((this->Position.y + this->Size.y) - rOrigin.y) * invdir.y;
		tymax = (this->Position.y - rOrigin.y) * invdir.y;
	}

	
	if ((tmin > tymax) || (tymin > tmax)) {
		return false;
	}

	if (tymin > tmin) {
		tmin = tymin;
	}

	if (tymax < tmax) {
		tmax = tymax;
	}

	lowestFraction = tmin;

	if (lowestFraction < 0) {
		lowestFraction = tmax;
		if (lowestFraction < 0) {
			return false;
		}
	}

	intersectionPoint = rOrigin + rDir * lowestFraction;

	return true;
}

bool Object::calculateAABBcollision(Object &object1, Object &object2) {
	//Check if collide on X axis.
	bool collisionX = object1.Position.x + object1.Size.x >= object2.Position.x && object2.Position.x + object2.Size.x >= object1.Position.x;
	//Check if collide on Y axis.
	bool collisionY = object1.Position.y + object1.Size.y >= object2.Position.y && object2.Position.y + object2.Size.y >= object1.Position.y;
	//If collision on both axes, then objects collide with eachother.
	return collisionX && collisionY;
}