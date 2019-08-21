#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include <string>
#include "object/object.h"
#include "object/lightObject/lightObject.h"
#include "renderers/spriteRenderer/SpriteRenderer.h"
#include "renderers/rayRenderer/RayRenderer.h"
#include "shader/shader.h"
#include "guiManager/guiManager.h"

class objectManager {
private:
	//group of object pointers that contain all the objects in the scene.
	std::vector<Object*> objList;
	std::vector<lightObject*> lightList;

	SpriteRenderer  *Renderer;
	RayRenderer *rRenderer;
	guiManager *gui;
	std::vector<glm::vec2> intersectionPoints;

	Object *selectedObject = nullptr;
	lightObject *selectedLight = nullptr;
	float nAir = 1.0f;

	float selectedObjectRotation;

	void fresnel(const glm::vec2 direction, const glm::vec2 normal, const float &rI, float &kr);
	glm::vec2 calculateNormal(glm::vec2 intersectionPoint, Object aabb);
	void traceRay(lightObject* currentLight, glm::vec2 &origin, glm::vec2 &direction, unsigned int depth);
	bool castRay(glm::vec2 &orig, glm::vec2 &dir, unsigned int &tIndex, Object** hitObject, glm::vec2 &intersectionPt);
	bool castRay3D(glm::vec3 &orig, glm::vec3 &dir, unsigned int &tIndex, Object** hitObject, glm::vec3 &intersectionPt);
	bool castRay3D(glm::vec3 &orig, glm::vec3 &dir, unsigned int &tIndex, lightObject** hitObject, glm::vec3 &intersectionPt);
	glm::vec2 doReflection(const glm::vec2 &incidentRay, const glm::vec2 &intersectionNormal);
	glm::vec2 doRefraction(const glm::vec2 &Incident, const glm::vec2 &Normal, float &n1, float &n2);
	void checkRefIndex(lightObject* theLight, glm::vec2 theOrigin);

public:
	objectManager(Shader quadShader, Shader rayShader);
	~objectManager();
	void addObject(glm::vec2 oPos, glm::vec2 oSize, std::string oTex);
	void addLight(glm::vec2 lPos, glm::vec2 lSize, std::string lTex);
	void selectObject(glm::vec3 coords, bool hasInput, bool eKey, bool rKey);
	void deselectObject();
	//void moveObject(glm::vec3 newCoords);
	void drawAll();
};

#endif
