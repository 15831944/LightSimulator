#include "guiManager.h"

guiManager::guiManager() {
	
}

void guiManager::prepareNewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void guiManager::renderNewFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void guiManager::createObjectDetailsWindow(Object *&ob, lightObject *&lightOb) {
	ImGui::Begin("Object Info");
	ImGui::SetWindowPos(objectInfoWindowPos);
	ImGui::SetWindowSize(objectInfoWindowSize);
	if (ob != nullptr) {
		ImGui::Text("Object");
		ImGui::InputFloat("Refractive Index", &ob->refractiveIndex, 0.01f, 0.01f, 3);
		ImGui::SliderFloat("Size x", &ob->Size.x, 10.0f, 500.0f);
		ImGui::SliderFloat("Size y", &ob->Size.y, 10.0f, 500.0f);
		ImGui::SliderFloat("Position x", &ob->Position.x, 0.0f, 1200.0f - ob->Size.x);
		ImGui::SliderFloat("Position y", &ob->Position.y, 0.0f, 1000.0f - ob->Size.y);
	}
	else if (lightOb != nullptr) {
		ImGui::Text("Colour");
		ImGui::SliderFloat("R", &lightOb->rayColour.r, 0.0f, 1.0f);
		ImGui::SliderFloat("G", &lightOb->rayColour.g, 0.0f, 1.0f);
		ImGui::SliderFloat("B", &lightOb->rayColour.b, 0.0f, 1.0f);
	}
	ImGui::End();
}

void guiManager::createSceneManagerWindow(bool &addOb, bool &addlightOb) {
	ImGui::Begin("Scene Manager");
	ImGui::SetWindowPos(sceneManagerWindowPos);
	ImGui::SetWindowSize(sceneManagerWindowSize);
	/*ImGui::Text("Background Colour");
	ImGui::SliderFloat("R", &background.r, 0.0f, 1.0f);
	ImGui::SliderFloat("G", &background.g, 0.0f, 1.0f);
	ImGui::SliderFloat("B", &background.b, 0.0f, 1.0f);*/
	if (ImGui::Button("Add Object")) {
		addOb = true;
	}
	else if (ImGui::Button("Add Light")) {
		addlightOb = true;
	}
	else {
		addOb = false;
		addlightOb = false;
	}
	ImGui::End();
}

void guiManager::createDemoWindow() {
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}