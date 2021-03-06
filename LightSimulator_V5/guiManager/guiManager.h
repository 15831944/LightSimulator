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

	const static bool no_titlebar = true;
	const static bool no_move = true;
	const static bool no_resize = true;
	ImGuiWindowFlags window_flags = 0;

	struct addObjData {
		glm::vec2 size = glm::vec2(200.0f, 200.0f);
		float refIndex = 1.5f;
		bool isMirror = false;
	}objData;

	struct addLightData {
		int noRays = 1;
		glm::vec3 colour = glm::vec3(1.0f);
		bool turnOff = false;
	}lightData;

	/*
	0 = No experiment
	1 = Multiple objects experiment
	*/
	int activeExperiment = 0;

	//For experiment 1
	float refractiveIndexes[6] = { 0.000, 0.000, 0.000, 0.000, 0.000, 0.000 };
	float incidenceAngle = 0.0f;
	int objectGap = 0;

	guiManager();
	void prepareNewFrame();
	void renderNewFrame();
	void createSceneManagerWindow(bool &clearScene, bool &addOb, bool &addlightOb, ObjectTemplate &object, std::vector<angleIndicator*> angleArray);
	void displayResults(std::vector<angleIndicator*> data);
	void displayNumbers(std::vector<angleIndicator*> data);
};
#endif // !GUIMANAGER_H