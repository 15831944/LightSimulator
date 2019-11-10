#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "OpenGL/imgui/imgui.h"
#include "OpenGL/imgui/imgui_impl_glfw.h"
#include "OpenGL/imgui/imgui_impl_opengl3.h"

#include "objectManager/object/object.h"
#include "objectManager/object/lightObject/lightObject.h"
#include "objectManager/object/angleIndicator/angleIndicator.h"
#include <vector>


class guiManager {
public:
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec2 sceneManagerWindowPos = ImVec2(10.0f, 10.0f);
	ImVec2 sceneManagerWindowSize = ImVec2(400.0f, 1000.0f);

	/*
	0 = No experiment
	1 = Multiple objects experiment
	*/
	int activeExperiment = 0;

	//For experiment 1
	float refractiveIndexes[6] = { 0.000, 0.000, 0.000, 0.000, 0.000, 0.000 };
	float incidenceAngle = 0.0f;

	guiManager();
	void prepareNewFrame();
	void renderNewFrame();
	void createSceneManagerWindow(bool clearScene, bool &addOb, bool &addlightOb, Object *&ob, lightObject *&lightOb, angleIndicator *&ind);
	void displayResults(std::vector<angleIndicator*> data);
};
#endif // !GUIMANAGER_H