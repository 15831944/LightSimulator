#include "objectManager.h"
#include <iostream>
#include <algorithm>
#include "resourceManager/resourceManager.h"

constexpr auto PI = 3.14159265358979323846264338327f;

inline float clamp(const float &lo, const float &hi, const float &v)
{
	return std::max(lo, std::min(hi, v));
}

objectManager::objectManager(Shader quadShader, Shader rayShader) {
	Renderer = new SpriteRenderer(quadShader);
	rRenderer = new RayRenderer(rayShader);
	gui = new guiManager();
}

objectManager::~objectManager() {
	delete Renderer;
	delete rRenderer;
	delete gui;
}

void objectManager::selectObject(glm::vec3 coords, bool hasInput, bool eKey, bool rKey) {
	//if the user clicks
	if (hasInput) {
		unsigned int index = 0;
		glm::vec3 intersectionPoint;
		glm::vec3 direct = glm::vec3(0.0f, 0.0f, -1.0f);
		if (selectedLight != nullptr && selectedObject != nullptr) {
			selectedObject = nullptr;
		}
		if (selectedObject == nullptr && selectedLight == nullptr) {
			for (unsigned int i = 0; i < objList.size(); i++) {
				if (castRay3D(coords, direct, index, &this->selectedObject, intersectionPoint)) {
					break;
				}
			}
		}
		if (selectedLight == nullptr) {
			for (unsigned int i = 0; i < lightList.size(); i++) {
				if (castRay3D(coords, direct, index, &this->selectedLight, intersectionPoint)) {
					break;
				}
			}
		}

		
	}
	//if no click
	else {
		if (selectedObject != nullptr && eKey) {
			if (selectedLight == nullptr) {
				this->selectedObject->moveObject(coords);
				//this->selectedObject->rotateObject();
			}
		}
		if (selectedLight != nullptr && eKey) {
			this->selectedLight->moveObject(coords);
		}
		if (selectedLight != nullptr && rKey) {
			this->selectedLight->rotateToMouse(coords);
		}
	}
}


void objectManager::deselectObject() {
	this->selectedObject = nullptr;
	this->selectedLight = nullptr;
}

void objectManager::addObject(glm::vec2 oPos, glm::vec2 oSize, std::string oTex) {
	this->objList.push_back(new Object(oPos, oSize, ResourceManager::GetTexture(oTex)));
}

void objectManager::addLight(glm::vec2 lPos, glm::vec2 lSize, std::string lTex) {
	this->lightList.push_back(new lightObject(lPos, lSize, ResourceManager::GetTexture(lTex)));
}

glm::vec2 objectManager::doReflection(const glm::vec2 &incidentRay, const glm::vec2 &intersectionNormal) {
	return incidentRay - 2 * glm::dot(incidentRay, intersectionNormal) * intersectionNormal;
}

glm::vec2 objectManager::doRefraction(const glm::vec2 &Incident, const glm::vec2 &Normal, float &n1, float &n2) {
	float cosI = clamp(-1, 1, glm::dot(Incident, Normal));
	float N1 = n1, N2 = n2;
	glm::vec2 norm = Normal;
	if (cosI < 0) {
		cosI = -cosI;
	}
	else {
		std::swap(N1, N2);
		norm = -Normal;
	}
	float n = N1 / N2;
	float sinT2 = 1 - (n * n) * (1 - (cosI * cosI));//If this is less than 0, then total internal reflection occurs.
	//If total internal reflection occurs, the call the reflection function to calculate the new direction vector. Otherwise, calculate the refracted ray's direction.
	return sinT2 < 0 ? doReflection(Incident, Normal) : n * Incident + (n * cosI - sqrtf(sinT2)) * norm;
}

void objectManager::fresnel(const glm::vec2 direction, const glm::vec2 normal, const float &rI, float &kr) {
	float cosi = clamp(-1, 1, glm::dot(direction, normal));
	float n1 = 1, n2 = rI; //ni = incident refractive index, nt = transmission frfractive index.

	if (cosi > 0) {
		std::swap(n1, n2);
	}
	
	//calculate sint using snell's law
	float sint = n1 / n2 * sqrtf(std::max(0.0f, 1 - cosi * cosi));

	//total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.0f, 1 - sint * sint));
		cosi = fabsf(cosi); //fabsf = floating point absolute value that takes in a float.
		
		//Reflectance for s-polarized light.
		float Rs = ((n1 * cosi) - (n1 * cost)) / ((n2 * cosi) + (n1 * cost));
		
		//Reflectance for p-polarized light.
		float Rp = ((n1 * cosi) - (n2 * cost)) / ((n1 * cosi) + (n2 * cost));
		
		kr = (Rs * Rs + Rp * Rp) / 2; //amount of reflected light (in range 0-1).
	}

}

//THIS IS THE ISSUE: FIX BY ALLOWING TO CALCULATE NORMAL OF INTERNAL INTERSECTIONS
glm::vec2 objectManager::calculateNormal(glm::vec2 intersectionPoint, Object aabb) {
	float epsilon = 0.01f;
	if (abs(intersectionPoint[0] - aabb.Position[0]) < epsilon) {
		//left side
		return glm::vec2(-1, 0);
	}
	else if (abs(intersectionPoint[0] - (aabb.Position[0] + aabb.Size[0])) < epsilon) {
		//right side
		return glm::vec2(1, 0);
	}
	else if (abs(intersectionPoint[1] - aabb.Position[1]) < epsilon) {
		//top side
		return glm::vec2(0, -1);
	}
	else if (abs(intersectionPoint[1] - (aabb.Position[1] + aabb.Size[1])) < epsilon) {
		//bottom side
		return glm::vec2(0, 1);
	}
	else {
		//only returns if the calculation fails.
		return glm::vec2(0, 0);
	}
}

bool objectManager::castRay3D(glm::vec3 &orig, glm::vec3 &dir, unsigned int &tIndex, Object** hitObject, glm::vec3 &intersectionPt) {
	glm::vec3 testIntersection;
	float lowestFraction = 1;
	float testLowestFraction;

	for (unsigned int i = 0; i < objList.size(); i++) {
		if (objList[i]->CalculateRayCollision3D(orig, dir, testIntersection, testLowestFraction) && testLowestFraction < lowestFraction) {
			*hitObject = objList[i];
			intersectionPt = testIntersection;
			lowestFraction = testLowestFraction;
			tIndex = i;
		}
	}

	if (lowestFraction < 1) {
		return true;
	}
	return false;
}

bool objectManager::castRay3D(glm::vec3 &orig, glm::vec3 &dir, unsigned int &tIndex, lightObject** hitObject, glm::vec3 &intersectionPt) {
	glm::vec3 testIntersection;
	float lowestFraction = 1;
	float testLowestFraction;

	for (unsigned int i = 0; i < lightList.size(); i++) {
		if (lightList[i]->CalculateRayCollision3D(orig, dir, testIntersection, testLowestFraction) && testLowestFraction < lowestFraction) {
			*hitObject = lightList[i];
			intersectionPt = testIntersection;
			lowestFraction = testLowestFraction;
			tIndex = i;
		}
	}

	if (lowestFraction < 1) {
		return true;
	}
	return false;
}

bool objectManager::castRay(glm::vec2 &orig, glm::vec2 &dir, unsigned int &tIndex, Object** hitObject, glm::vec2 &intersectionPt) {
	glm::vec2 testIntersection;
	float lowestFraction = 2000.0f;
	float testLowestFraction;

	for (unsigned int i = 0; i < objList.size(); i++) {
		if (objList[i]->CalculateRayCollision(orig, dir, testIntersection, testLowestFraction) && testLowestFraction < lowestFraction) {
			*hitObject = objList[i];
			intersectionPt = testIntersection;
			lowestFraction = testLowestFraction;
			tIndex = i;
		}
	}

	if (lowestFraction < 2000.0f) {
		return true;
	}
	return false;
}

void objectManager::checkRefIndex(lightObject* theLight, glm::vec2 theOrigin) {
	for (unsigned int i = 0; i < objList.size(); i++) {
		if (objList[i]->isRayInsideObject(theOrigin)) {
			theLight->currentRefractiveIndex = objList[i]->refractiveIndex;
		}
		else {
			theLight->currentRefractiveIndex = 1.0f;
		}
	}
}

void objectManager::traceRay(lightObject* currentLight, glm::vec2 &origin, glm::vec2 &direction, unsigned int depth) {	
	
	if (depth > 40) {
		checkRefIndex(currentLight, origin);
		currentLight->DrawRay(*rRenderer, origin, origin + direction * 2000.0f);
	}
	else {
		unsigned int index = 0;
		Object *hitObject = nullptr;
		glm::vec2 intersectionPoint;
		if (castRay(origin, direction, index, &hitObject, intersectionPoint)) {
			currentLight->DrawRay(*rRenderer, origin, intersectionPoint);
			glm::vec2 normal = calculateNormal(intersectionPoint, *hitObject);

			//Reflection and Refraction
			glm::vec2 reflectionDirection = glm::normalize(doReflection(direction, normal));
			glm::vec2 refractionDirection = glm::normalize(doRefraction(direction, normal, currentLight->currentRefractiveIndex, hitObject->refractiveIndex));
			
			glm::vec2 reflectionRayOrigin = (glm::dot(reflectionDirection, normal) < 0) ? intersectionPoint - normal * 0.01f : intersectionPoint + normal * 0.01f;
			glm::vec2 refractionRayOrigin = (glm::dot(glm::normalize(refractionDirection), normal) < 0) ? intersectionPoint - normal * 0.01f : intersectionPoint + normal * 0.01f;

			traceRay(currentLight, refractionRayOrigin, refractionDirection, depth + 1);
			//traceRay(currentLight, reflectionRayOrigin, reflectionDirection, depth + 1);
		}
		else {
			currentLight->DrawRay(*rRenderer, origin, origin + direction * 2000.0f);
		}
	}
}

void objectManager::drawAll() {
	for (unsigned int i = 0; i < objList.size(); i++) {
		objList[i]->Draw(*Renderer);
	}
	for (unsigned int i = 0; i < lightList.size(); i++) {
		lightList[i]->Draw(*Renderer);
		checkRefIndex(lightList[i], lightList[i]->rayOrigin);
		traceRay(lightList[i], lightList[i]->rayOrigin, lightList[i]->rayDirection, 0);
	}
	bool addLight = false;
	bool addObject = false;
	gui->prepareNewFrame();
	gui->createObjectDetailsWindow(selectedObject, selectedLight);
	gui->createSceneManagerWindow(addObject, addLight);
	gui->renderNewFrame();
	if (addLight) {
		this->addLight(glm::vec2(300.0f, 200.0f), glm::vec2(40.0f, 40.0f), "block");
	}
	else if (addObject) {
		this->addObject(glm::vec2(400.0f, 200.0f), glm::vec2(100.0f, 100.0f), "sblock");
	}
}