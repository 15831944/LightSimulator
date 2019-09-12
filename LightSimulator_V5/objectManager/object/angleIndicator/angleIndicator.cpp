#include "angleIndicator.h"

angleIndicator::angleIndicator(float angle1, float angle2, float n1, float n2, glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec4 color)
	:ObjectTemplate(position, size, sprite, color) {
	Position.x = position.x - (size.x / 2);
	Position.y = position.y - (size.y / 2);
	this->n1 = n1;
	this->n2 = n2;
	iAngle = angle1;
	rAngle = angle2;
}