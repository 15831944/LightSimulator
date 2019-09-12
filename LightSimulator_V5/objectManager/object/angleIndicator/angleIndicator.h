#ifndef  ANGLEINDICATOR_H
#define ANGLEINDICATOR_H

#include "objectManager/object/objectTemplate.h"
#include "renderers/spriteRenderer/SpriteRenderer.h"

class angleIndicator : public ObjectTemplate
{
public:
	float iAngle, rAngle, n1, n2; //Create variables to hold the angles of incidence and reflection/refraction. Also vaiables to hold the refractive indexes involved.
	angleIndicator(float angle1, float angle2, float n1, float n2, glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f));
};

#endif // ! ANGLEINDICATOR_H
