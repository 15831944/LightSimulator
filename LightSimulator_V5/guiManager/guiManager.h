#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "OpenGL/imgui/imgui.h"
#include "OpenGL/imgui/imgui_impl_glfw.h"
#include "OpenGL/imgui/imgui_impl_opengl3.h"

#include "objectManager/object/object.h"
#include "objectManager/object/lightObject/lightObject.h"
#include "objectManager/object/angleIndicator/angleIndicator.h"

class guiManager {
public:
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec2 objectInfoWindowPos = ImVec2(800.0f, 10.0f);
	ImVec2 objectInfoWindowSize = ImVec2(400.0f, 200.0f);
	ImVec2 sceneManagerWindowPos = ImVec2(10.0f, 10.0f);
	ImVec2 sceneManagerWindowSize = ImVec2(200.0f, 200.0f);

	guiManager();
	void prepareNewFrame();
	void renderNewFrame();
	void createObjectDetailsWindow(Object *&ob, lightObject *&lightOb, angleIndicator *&ind);
	void createSceneManagerWindow(bool &addOb, bool &addlightOb);
	void createDemoWindow();
};
#endif // !GUIMANAGER_H