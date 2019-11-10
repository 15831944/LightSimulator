#include "lightObject.h"
#include "resourceManager/resourceManager.h"
#include "objectManager/objectManager.h"
#include <algorithm>
#include <math.h>
#include <iostream>

constexpr auto PI = 3.14159265358979323846264338327f;

lightObject::lightObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, bool fixed)
	: ObjectTemplate(pos, size, sprite, glm::vec4(1.0f), fixed) {
	//Set default ray colour.
	rayColour = glm::vec3(1.0f, 1.0f, 1.0f);
	
	//Set default rayDirection.
	rayDirection = glm::normalize(glm::vec2(1.0f, 0.0f));

	//set ray origin points at center of light object.
	rayOrigin.x = 0.5f * size.x + pos.x;
	rayOrigin.y = 0.5f * size.y + pos.y;
	//Add an object that the lightObject will rotate towards.
	directionIndicator = new ObjectTemplate(rayOrigin + rayDirection * 100.0f, glm::vec2(20.0f, 20.0f), ResourceManager::GetTexture("angleIndicator"));
	rotateToIndicator();
}

void lightObject::Draw(SpriteRenderer &renderer)
{
	rotateToIndicator(); //Call the rotate function here so that the ray direction updates while the indicator is being moved by the user.
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	renderer.DrawSprite(this->directionIndicator->Sprite, this->directionIndicator->Position, this->directionIndicator->Size, this->directionIndicator->Rotation, this->directionIndicator->Color);
}

void lightObject::DrawRay(RayRenderer &renderer, glm::vec2 v0, glm::vec2 v1) {
	renderer.Draw(v0, v1, this->rayColour);
}

void lightObject::moveObject(glm::vec2 newPos) {
	this->Position = glm::vec3(newPos, 0.0f);
	this->rayOrigin.x = 0.5f * this->Size.x + newPos.x;
	this->rayOrigin.y = 0.5f * this->Size.y + newPos.y;
}

void lightObject::rotateObject(float angle) {
	//float angleRadians = angle * PI / 180.0f;
	this->Rotation = angle;
	//this->rotateRay(angle);
}

float lightObject::calculateAngle(glm::vec2 rayDir, glm::vec2 normal) {
	//calculate the angle between ray and normal. Since both vectors are unit vectors, they don't need to be normalized.
	return (acos(glm::dot(rayDir, normal)));	//use dot product with arc-cosine to find angle, angle is in radians.
}

void lightObject::rotateToMouse(glm::vec3 coords) {
	glm::vec2 coords2D = glm::vec2(coords.x, coords.y);
	glm::vec2 directionVec = glm::normalize(coords2D - this->rayOrigin);//calculate new direction vector.
	float angle = calculateAngle(directionVec, glm::vec2(0.0f, -1.0f));//calc angle between north and direction vector.
	
	if (directionVec.x < 0) //if thex value is negative, then the angle is reflex.
	{
		this->rotateObject((PI * 2) - angle);//calculate reflex angle (from PI to 2*PI).
	}
	else {
		this->rotateObject(angle);//calculate normal angle (from 0 to PI).
	}
	this->rayDirection = directionVec;//set new direction vector for the light ray.
}

void lightObject::rotateToIndicator() {
	glm::vec2 coords2D = glm::vec2(this->directionIndicator->Position.x + (this->directionIndicator->Size.x / 2), this->directionIndicator->Position.y + (this->directionIndicator->Size.y / 2));
	glm::vec2 directionVec = glm::normalize(coords2D - this->rayOrigin);//calculate new direction vector.
	float angle = calculateAngle(directionVec, glm::vec2(0.0f, -1.0f));//calc angle between north and direction vector.

	if (directionVec.x < 0) //if thex value is negative, then the angle is reflex.
	{
		this->rotateObject((PI * 2) - angle);//calculate reflex angle (from PI to 2*PI).
	}
	else {
		this->rotateObject(angle);//calculate normal angle (from 0 to PI).
	}
	this->rayDirection = directionVec;//set new direction vector for the light ray.
}

void lightObject::rotateByAngle(float angle) {
	float temp = rayDirection.x;
	rayDirection.x = rayDirection.x * cos(glm::radians(angle)) + rayDirection.y * -sin(glm::radians(angle));
	rayDirection.y = temp * sin(glm::radians(angle)) + rayDirection.y * cos(glm::radians(angle));
	this->directionIndicator->Position = glm::vec3(this->rayOrigin + this->rayDirection * glm::vec2(50), 0.0f);
}