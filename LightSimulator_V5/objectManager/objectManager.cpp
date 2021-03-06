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
	selState = choose;
	//dbMan = *new DatabaseManager();
}

objectManager::~objectManager() {
	delete Renderer;
	delete rRenderer;
	delete gui;
}

void objectManager::selectObject(glm::vec3 coords, bool mouse1, bool mouse2) {
	unsigned int index = 0;
	glm::vec3 intersectionPoint = glm::vec3(0.0f);
	glm::vec3 direct = glm::vec3(0.0f, 0.0f, -1.0f);
	if (mouse2) {//Right mouse button
		//use ray casting along the Z axis from the cursor's position to see if the ray intersects 
		//with anything:
		if (castRay3D(coords, direct, index, this->selectedObject, intersectionPoint)) {
			deleteTheObject(selectedObject);
		}
	}
	if (mouse1 && selState == choose) {//Left mouse button is clicked and the state is set to choosing objects.
		deselectObject();//clear any currently selected objects.
		if (!isSelected) {
			if (castRay3D(coords, direct, index, this->selectedObject, intersectionPoint)) {//Use Z axis ray cast.
				//Allows object to be moved by mouse from anywhere, not just the origin of the object.
				positionOffset = intersectionPoint - this->selectedObject->Position;
				isSelected = true;
			}
		}
		selState = move;//Change state to allow object to be moved by the mouse.
	}
	if (selState == move) {
		if (coords.x > 410.0f) {//Make sure that the object can't be moved behind the GUI.
			if (isSelected) {
				//Check to make sure the object hasn't been marked as not movable. If it is, then don't move object.
				if (!selectedObject->FixedPosition) {
					this->selectedObject->moveObject(coords - positionOffset);
				}
			}

		}

		if (!mouse1) {
			selState = edit;
		}
	}
	if (selState == edit) {
		if (mouse1 && coords.x > 410.0f) {
			selState = choose;//when the user clicks the mouse again, attempt to select another object.
		}
		
	}
}


void objectManager::deselectObject() {
	isSelected = false;
}

void objectManager::addObject(glm::vec2 oPos, glm::vec2 oSize, std::string oTex, glm::vec4 color, bool fix) {
	//add a new object to the end of the objList array.
	this->objList.push_back(new Object(oPos, oSize, ResourceManager::GetTexture(oTex), color, fix));
	//read refractive index from the guiManager's objData struct.
	objList.back()->refractiveIndex = gui->objData.refIndex;
	if (gui->objData.isMirror) {//if it's a mirror
		objList.back()->material = Object::mirror;//Set the material
		objList.back()->Sprite = ResourceManager::GetTexture("mirror");//Use the appropriate texture.
	}
	else {
		objList.back()->material = Object::opaque;//Otherwise, set the material to opaque.
	}
	
}

void objectManager::addLight(glm::vec2 lPos, glm::vec2 lSize, std::string lTex, bool fix) {
	//add new light object to the end of the lightList array.
	this->lightList.push_back(new lightObject(lPos, lSize, ResourceManager::GetTexture(lTex), fix));
	//add the direction indicator to the dirList.
	this->dirList.push_back(lightList[lightList.size()-1]->directionIndicator);
	//Set parameters from the guiManager.
	lightList.back()->rayColour = gui->lightData.colour;
	lightList.back()->noOfRays = gui->lightData.noRays;
	lightList.back()->turnedOff = gui->lightData.turnOff;
}

void objectManager::addAngleIndicator(float angle1, float angle2, float n1, float n2, glm::vec2 aPos, glm::vec2 aSize, std::string aTex) {
	this->angList.push_back(new angleIndicator(angle1, angle2, n1, n2, aPos, aSize, ResourceManager::GetTexture(aTex)));
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

//Used to calculate reflectance of reflected light.
void objectManager::fresnel(const glm::vec2 direction, const glm::vec2 normal, const float &rI, float &kr) {
	float cosi = clamp(-1, 1, glm::dot(direction, normal));
	float n1 = 1, n2 = rI; //ni = incident refractive index, nt = transmission frfractive index.

	if (cosi > 0) {
		std::swap(n1, n2);
	}
	
	//calculate sin of transmitted light using snell's law
	float sint = n1 / n2 * sqrtf(std::max(0.0f, 1 - cosi * cosi));

	//total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.0f, 1 - sint * sint));
		cosi = fabsf(cosi); //fabsf = absolute value of a float.
		
		//Reflectance for s-polarized light.
		float Rs = ((n1 * cosi) - (n1 * cost)) / ((n2 * cosi) + (n1 * cost));
		
		//Reflectance for p-polarized light.
		float Rp = ((n1 * cosi) - (n2 * cost)) / ((n1 * cosi) + (n2 * cost));
		
		kr = (Rs * Rs + Rp * Rp) / 2; //amount of reflected light (in range 0-1).
	}
}

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

bool objectManager::castRay3D(glm::vec3 &orig, glm::vec3 &dir, unsigned int &tIndex, ObjectTemplate*& hitObject, glm::vec3 &intersectionPt) {
	glm::vec3 testIntersection;
	float lowestFraction = 1;
	float testLowestFraction;

	for (unsigned int i = 0; i < dirList.size(); i++) {
		if (dirList[i]->CalculateRayCollision3D(orig, dir, testIntersection, testLowestFraction) && testLowestFraction < lowestFraction) {
			hitObject = dirList[i];
			intersectionPt = testIntersection;
			lowestFraction = testLowestFraction;
			tIndex = i;
		}
	}

	for (unsigned int i = 0; i < angList.size(); i++) {
		if (angList[i]->CalculateRayCollision3D(orig, dir, testIntersection, testLowestFraction) && testLowestFraction < lowestFraction) {
			hitObject = angList[i];
			intersectionPt = testIntersection;
			lowestFraction = testLowestFraction;
			tIndex = i;
		}
	}

	for (unsigned int i = 0; i < objList.size(); i++) {
		if (objList[i]->CalculateRayCollision3D(orig, dir, testIntersection, testLowestFraction) && testLowestFraction < lowestFraction) {
			hitObject = objList[i];
			intersectionPt = testIntersection;
			lowestFraction = testLowestFraction;
			tIndex = i;
		}
	}

	for (unsigned int i = 0; i < lightList.size(); i++) {
		if (lightList[i]->CalculateRayCollision3D(orig, dir, testIntersection, testLowestFraction) && testLowestFraction < lowestFraction) {
			hitObject = lightList[i];
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

bool objectManager::traceRay(glm::vec2 &orig, glm::vec2 &dir, unsigned int &tIndex, Object** hitObject, glm::vec2 &intersectionPt) {
	glm::vec2 testIntersection;
	float lowestFraction = 2000.0f, testLowestFraction;

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

//Makes sure that ray tracing works when the ray origin is inside an object.
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

float objectManager::calcAngle(glm::vec2 vec1, glm::vec2 vec2) {
	return (std::acos(glm::dot(vec1, vec2)) * 180/3.141592654); //No need to divide by magnitude since both vec1 and vec2 are unit vectors.
}

void objectManager::castRay(lightObject* currentLight, glm::vec2 &origin, glm::vec2 &direction, float alpha, unsigned int depth, int id = 0) {
	
	if (depth > 20 || alpha < 0.01f) {
		checkRefIndex(currentLight, origin);
		currentLight->DrawRay(*rRenderer, origin, origin + direction * 2000.0f, alpha, id);
	}
	else {
		unsigned int index = 0;
		Object *hitObject = nullptr;
		glm::vec2 intersectionPoint, angleIntersectionPoint, reflectionDirection, refractionDirection, reflectionRayOrigin, refractionRayOrigin;
		glm::vec3 checkPoint = glm::vec3(angleIntersectionPoint, 1.0f), intersection = glm::vec3(0.0f), direc = glm::vec3(0.0f, 0.0f, -1.0f);
		float angle1, angle2, reflectance;

		if (traceRay(origin, direction, index, &hitObject, intersectionPoint)) {

			angleIntersectionPoint = intersectionPoint;
			currentLight->DrawRay(*rRenderer, origin, intersectionPoint, alpha, id);
			glm::vec2 normal = calculateNormal(intersectionPoint, *hitObject);
			//draw normal
			//currentLight->DrawRay(*rRenderer, intersectionPoint - normal * 50.0f, intersectionPoint + normal * 50.0f);
			//Reflection and Refraction
			switch (hitObject->material) {
			case Object::opaque:
				reflectionDirection = glm::normalize(doReflection(direction, normal));
				refractionDirection = glm::normalize(doRefraction(direction, normal, currentLight->currentRefractiveIndex, hitObject->refractiveIndex));
				
				reflectionRayOrigin = (glm::dot(reflectionDirection, normal) < 0) ? intersectionPoint - normal * 0.01f : intersectionPoint + normal * 0.01f;
				refractionRayOrigin = (glm::dot(glm::normalize(refractionDirection), normal) < 0) ? 
					intersectionPoint - normal * 0.01f : intersectionPoint + normal * 0.01f;

				if (glm::dot(glm::normalize(refractionDirection), normal) < 0) {
					angleIntersectionPoint += normal * 0.01f;
				}
				else {
					angleIntersectionPoint -= normal * 0.01f;
				}

				
				ObjectTemplate *temp;
				if (castRay3D(checkPoint, direc, index, temp, intersection)) {
					this->thing = (Object*)temp;
					angle1 = calcAngle(normal, direction);
					angle2 = calcAngle(normal, refractionDirection);
				}
				else {
					angle1 = calcAngle(normal, -direction);
					angle2 = calcAngle(normal, -refractionDirection);
				}
				if (currentLight->noOfRays == 1) {
					addAngleIndicator(angle1, angle2, currentLight->currentRefractiveIndex, hitObject->refractiveIndex, intersectionPoint, 
						glm::vec2(30.0f, 30.0f), "angleIndicator");
				}

				fresnel(direction, normal, hitObject->refractiveIndex, reflectance);

				castRay(currentLight, refractionRayOrigin, refractionDirection, alpha - reflectance, depth + 1, id);
				castRay(currentLight, reflectionRayOrigin, reflectionDirection, alpha - (1 - reflectance), depth + 1, id);
				break;

			case Object::mirror:
				reflectionDirection = glm::normalize(doReflection(direction, normal));
				reflectionRayOrigin = (glm::dot(reflectionDirection, normal) < 0) ? intersectionPoint - normal * 0.01f : intersectionPoint + normal * 0.01f;
				float angle = calcAngle(normal, direction);

				if (currentLight->noOfRays == 1) {
					addAngleIndicator(angle, angle, currentLight->currentRefractiveIndex, hitObject->refractiveIndex, intersectionPoint, glm::vec2(30.0f, 30.0f), 
						"angleIndicator");
				}
				fresnel(direction, normal, hitObject->refractiveIndex, reflectance);

				castRay(currentLight, reflectionRayOrigin, reflectionDirection, alpha, depth + 1);
				break;
			}
			
		}
		else {
			currentLight->DrawRay(*rRenderer, origin, origin + direction * 2000.0f, alpha, id);
		}
	}
}

bool objectManager::doExperiment() {
	//This function will return a boolean which will tell whether an experiment was carried out.
	//Use of a switch statement to check which experiment to do.
	switch (gui->activeExperiment) {//the gui object contains an int to represent which experiment to do.
	case 1:	
		clearAllObjects();
		//Set the starting position for automatically added objects.
		glm::vec2 positions = glm::vec2(700, 500);
		//Loop through the user entered data from the GUI and add objects accordingly.
		for (unsigned int i = 0; i < sizeof(gui->refractiveIndexes)/sizeof(gui->refractiveIndexes[i]); i++) {
			if (gui->refractiveIndexes[i] != 0.000) {
				//Increment x coordinate so objects are added next to eachother.
				positions.x += 100 + gui->objectGap;
				//Add object with the fixed parameter as true to prevent the user from moving them accidentally.
				addObject(positions, glm::vec2(100, 500), "opaque", glm::vec4(1.0f), true);
				//Set the refractive indexes for each added object using the data entered in the GUI.
				objList[i]->refractiveIndex = abs(gui->refractiveIndexes[i]);
			}
		}
		//add a new light object with angle according to the users input. Set the fixed value to true to prevent accidental movement.
		addLight(glm::vec2(500, 300), glm::vec2(60, 60), "torch", true);
		lightList[0]->rotateByAngle(gui->incidenceAngle);
		
		//Now that experiment has been carried out, set the active experiment to 0 (no experiment) to prevent 
		//the next render loop repeating the experiment.
		gui->activeExperiment = 0;
		return true;
		break;


	default:
		return false;
		break;
	}
}

void objectManager::deleteTheObject(ObjectTemplate *unwanted) {
	//Convert base objectTemplate to the child class.
	Object* obj = dynamic_cast<Object*>(unwanted);
	lightObject* lObj = dynamic_cast<lightObject*>(unwanted);
	//Check which type the unwanted object is; light or object?
	if (obj != nullptr) {
		//Loop through the array of objects
		for (unsigned int i = 0; i < objList.size(); i++) {
			//If the object in the array matches the unwanted object, then erase it.
			if (obj == objList[i]) {
				objList.erase(objList.begin() + i);
			}
		}
	}
	else if (lObj != nullptr) {
		for (unsigned int i = 0; i < lightList.size(); i++) {
			if (lObj == lightList[i]) {
				lightList.erase(lightList.begin() + i);
			}
		}
	}
}


void objectManager::clearAllObjects() {
	//This function will erase all of the objects on the scene.
	objList.clear();
	lightList.clear();
}

void objectManager::drawAll() {
	angList.clear();//Clear any angle indicator objects from the previous frame.
	bool experimentDone = false;
	if (doExperiment()) {
		experimentDone = true;
	}

	//draw all objects.
	for (unsigned int i = 0; i < objList.size(); i++) {
		if (objList[i]->material == Object::opaque) {
			objList[i]->Sprite = ResourceManager::GetTexture("opaque");
		}
		else {
			objList[i]->Sprite = ResourceManager::GetTexture("mirror");
		}
		objList[i]->Draw(*Renderer);
	}

	

	//draw all light sources and carry out ray tracing.
	for (unsigned int i = 0; i < lightList.size(); i++) {
		//Draw light source
		lightList[i]->Draw(*Renderer);
		//check if the light is turned off
		if (!lightList[i]->turnedOff) {
			//Check if the number of rays is more than 1
			if (lightList[i]->noOfRays == 1) {
				checkRefIndex(lightList[i], lightList[i]->rayOrigin);
				castRay(lightList[i], lightList[i]->rayOrigin, lightList[i]->rayDirection, 1.0f, 0);
			}
			else {
				//If the number of rays is more than one, call the createOrigins() function to calculate where to place
				//each ray
				lightList[i]->createOrigins();
				for (int j = 0; j < lightList[i]->noOfRays; j++) {
					checkRefIndex(lightList[i], lightList[i]->rayOrigin);
					castRay(lightList[i], lightList[i]->rayOrigins[j], lightList[i]->rayDirection, 1.0f, 0, j+1);
				}
				lightList[i]->rayOrigins.clear();
			}
		}
	}
	//Render the angle indicators. The user can click on these to view infomation about what happened to the ray after it hit the surface.
	//for (unsigned int i = 0; i < angList.size(); i++) {
		//angList[i]->Draw(*Renderer);
	//}
	//Booleans used for adding to/editing the scene.
	bool addLight = false;
	bool addObject = false;
	bool clearSc = false;
	//Render GUI.
	gui->prepareNewFrame();
	gui->createSceneManagerWindow(clearSc, addObject, addLight, *selectedObject, angList);
	gui->displayNumbers(angList);
	gui->renderNewFrame();
	//If the user clicks to add an object, then the object will be added and rendered to the scene on the next frame.
	if (addLight) {
		this->addLight(glm::vec2(500.0f, 200.0f), glm::vec2(60.0f, 60.0f), "torch");
	}
	else if (addObject) {
		this->addObject(glm::vec2(800.0f, 200.0f), gui->objData.size, "opaque");
	}
	else if (clearSc) {
		clearAllObjects();
	}
}