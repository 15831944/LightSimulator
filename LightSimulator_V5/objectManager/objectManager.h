#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>
#include <string>
#include "object/object.h"
#include "object/lightObject/lightObject.h"
#include "objectManager/object/angleIndicator/angleIndicator.h"
#include "renderers/spriteRenderer/SpriteRenderer.h"
#include "renderers/rayRenderer/RayRenderer.h"
#include "shader/shader.h"
#include "guiManager/guiManager.h"
#include "databaseManager/dbManager.h"

class objectManager {
private:
	//DatabaseManager dbMan;

	//vector of object pointers that contain all the objects in the scene.
	std::vector<Object*> objList;
	std::vector<lightObject*> lightList;
	std::vector<angleIndicator*> angList;
	std::vector<ObjectTemplate*> dirList;

	SpriteRenderer  *Renderer;
	RayRenderer *rRenderer;
	guiManager *gui;
	std::vector<glm::vec2> intersectionPoints;
	
	//Holds the currently selected object, if any.
	ObjectTemplate* selectedObject;
	bool isSelected = false;

	glm::vec3 positionOffset = glm::vec3(0.0f);

	Object *thing = nullptr;

	enum selectState {
		choose,
		move,
		rotate,
		edit
	}selState;

	//Refractive index of air.
	float nAir = 1.0f;

	float selectedObjectRotation;
	bool mouseOverButton(glm::vec3 &orig, glm::vec3 &dir, unsigned int &tIndex, glm::vec3 &intersectionPt);
	void fresnel(const glm::vec2 direction, const glm::vec2 normal, const float &rI, float &kr);
	glm::vec2 calculateNormal(glm::vec2 intersectionPoint, Object aabb);
	void castRay(lightObject* currentLight, glm::vec2 &origin, glm::vec2 &direction, float alpha, unsigned int depth, int id);
	bool traceRay(glm::vec2 &orig, glm::vec2 &dir, unsigned int &tIndex, Object** hitObject, glm::vec2 &intersectionPt);
	bool castRay3D(glm::vec3 &orig, glm::vec3 &dir, unsigned int &tIndex, ObjectTemplate*& hitObject, glm::vec3 &intersectionPt);
	glm::vec2 doReflection(const glm::vec2 &incidentRay, const glm::vec2 &intersectionNormal);
	glm::vec2 doRefraction(const glm::vec2 &Incident, const glm::vec2 &Normal, float &n1, float &n2);
	void checkRefIndex(lightObject* theLight, glm::vec2 theOrigin);
	float calcAngle(glm::vec2 vec1, glm::vec2 vec2);
	bool doExperiment();
	void clearAllObjects();
	void deleteTheObject(ObjectTemplate* unwanted);

public:
	objectManager(Shader quadShader, Shader rayShader);
	~objectManager();
	void addObject(glm::vec2 oPos, glm::vec2 oSize, std::string oTex, glm::vec4 color = glm::vec4(1.0f) , bool fix = false);
	void addLight(glm::vec2 lPos, glm::vec2 lSize, std::string lTex, bool fix = false);
	void addAngleIndicator(float angle1, float angle2, float n1, float n2, glm::vec2 aPos, glm::vec2 aSize, std::string aTex);
	void selectObject(glm::vec3 coords, bool mouse1, bool mouse2);
	void deselectObject();
	void drawAll();
};

#endif
