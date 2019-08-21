#include "object.h"
#include <algorithm>
#include <math.h>
#include <iostream>

Object::Object()
	: Position(0, 0, 0), Size(1, 1, 0), Color(1.0f), Rotation(0.0f), Sprite() { }

Object::Object(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color)
	: Position(pos, 0.0f), Size(size, 0.0f), Color(color), Rotation(0.0f), Sprite(sprite) { 
	this->refractiveIndex = 2.417f;//diamond
}

void Object::moveObject(glm::vec2 newPos) {
	this->Position = glm::vec3(newPos, 0.0f);
}

void Object::rotateObject(float angle) {
	//do object rotation. AABB intersection needs to be updated for this.
}

bool Object::clipLine(int dimension, const glm::vec3& origin, const glm::vec3& end, float& f_low, float& f_high) {
	float f_dim_low, f_dim_high; //dimension specific low and high values.

	f_dim_high = (this->Position[dimension] - origin[dimension]) / (end[dimension] - origin[dimension]);
	f_dim_low = ((this->Position[dimension] + this->Size[dimension]) - origin[dimension]) / (end[dimension] - origin[dimension]);
	

	if (f_dim_high < f_dim_low) {
		float temp = f_dim_high;
		f_dim_high = f_dim_low;
		f_dim_low = temp;
	}

	if (f_dim_high < f_low) {
		return false;
	}

	if (f_dim_low > f_high) {
		return false;
	}

	f_low = std::max(f_dim_low, f_low);
	f_high = std::min(f_dim_high, f_high);

	if (f_low > f_high) {
		return false;
	}

	return true;
}

bool Object::isRayInsideObject(glm::vec2 &rOrigin) {
	float f_low = 0.0f;
	float f_high = 1.0f;

	//check if the ray intersects on both axis. If both functions return true, then the ray has hit an object.
	if (!clipLine(0, glm::vec3(rOrigin, 0.0f), glm::vec3(rOrigin, 0.0f), f_low, f_high)) {
		return false;
	}

	if (!clipLine(1, glm::vec3(rOrigin, 0.0f), glm::vec3(rOrigin, 0.0f), f_low, f_high)) {
		return false;
	}
	return true;
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



//Used for selecting objects with the mouse.
bool Object::CalculateRayCollision3D(glm::vec3 &rOrigin, glm::vec3 &rDir, glm::vec3& intersectionPoint, float& lowestFraction) {
	float f_low = 0.0f;
	float f_high = 1.0f;

	glm::vec3 rayEnd = rOrigin + rDir * 2000.0f;

	//check if the ray intersects on both axis. If both functions return true, then the ray has hit an object.
	if (!clipLine(0, rOrigin, rayEnd, f_low, f_high)) {
		return false;
	}

	if (!clipLine(1, rOrigin, rayEnd, f_low, f_high)) {
		return false;
	}

	if (!clipLine(2, rOrigin, rayEnd, f_low, f_high)) {
		return false;
	}

	glm::vec3 b = rayEnd - rOrigin;	//b is the vector of the casted ray.
	intersectionPoint = rOrigin + b * f_low; //equation of the line segment from origin to intersection. f_low will cut the line down to where it intersects.
	lowestFraction = f_low;	//keep track of lowestFraction incase a closer object is hit further in the loop.
	return true;
}

void Object::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}