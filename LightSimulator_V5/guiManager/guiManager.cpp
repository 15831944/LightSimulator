#include "guiManager.h"
#include <cstdio>

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

void guiManager::createSceneManagerWindow(bool &clearScene, bool &addOb, bool &addlightOb, ObjectTemplate &object, std::vector<angleIndicator*> angleArray) {
	if (no_titlebar)		window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Scene Manager", (bool*)0, window_flags);
	ImGui::SetWindowPos(sceneManagerWindowPos);
	ImGui::SetWindowSize(sceneManagerWindowSize);
	
	ImGui::BeginTabBar("tabBar");
	if (ImGui::BeginTabItem("Object Info")) {
		Object* ob = dynamic_cast<Object*>(&object);
		lightObject* lightOb = dynamic_cast<lightObject*>(&object);
		angleIndicator* ind = dynamic_cast<angleIndicator*>(&object);

		if (ob != nullptr) {
			ImGui::Text("Object");
			ImGui::InputFloat("Refractive Index", &ob->refractiveIndex, 0.01f, 0.01f, 3);
			ImGui::SliderFloat("Size x", &ob->Size.x, 10.0f, 500.0f);
			ImGui::SliderFloat("Size y", &ob->Size.y, 10.0f, 500.0f);
			ImGui::SliderFloat("Position x", &ob->Position.x, 0.0f, 1200.0f - ob->Size.x);
			ImGui::SliderFloat("Position y", &ob->Position.y, 0.0f, 1000.0f - ob->Size.y);
			ImGui::Checkbox("Fixed", &ob->FixedPosition);
		}
		else if (lightOb != nullptr) {
			ImGui::Text("Colour");
			ImGui::SliderFloat("R", &lightOb->rayColour.r, 0.0f, 1.0f);
			ImGui::Text("");
			ImGui::SliderFloat("G", &lightOb->rayColour.g, 0.0f, 1.0f);
			ImGui::Text("");
			ImGui::SliderFloat("B", &lightOb->rayColour.b, 0.0f, 1.0f);
			ImGui::Text("");
			ImGui::Text("");
			ImGui::SliderInt("Rays", &lightOb->noOfRays, 1, 300);
			ImGui::Text("");
			ImGui::Checkbox("Fixed", &lightOb->FixedPosition);
			ImGui::Text("");
			ImGui::Checkbox("Turn Off", &lightOb->turnedOff);
			
		}
		else if (ind != nullptr) {
			std::string a1, a2, ri1, ri2;

			a1 = "Incident angle: " + (std::to_string(*&ind->iAngle));
			a2 = "Reflected/Refracted angle: " + (std::to_string(*&ind->rAngle));
			ri1 = "Refractive index 1: " + (std::to_string(*&ind->n1));
			ri2 = "Refractive index 2: " + (std::to_string(*&ind->n2));

			ImGui::Text(a1.c_str());
			ImGui::Text("");
			ImGui::Text(a2.c_str());
			ImGui::Text("");
			ImGui::Text(ri1.c_str());
			ImGui::Text("");
			ImGui::Text(ri2.c_str());
		}
		else {
			ImGui::Text("No object selected!");
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Scene Settings")) {
		ImGui::Text("ENTER DATA FOR NEW OBJECT:");
		ImGui::Checkbox("mirror", &objData.isMirror);
		ImGui::InputFloat("Refractive Index", &objData.refIndex);
		ImGui::SliderFloat("Size x", &objData.size.x, 10.0f, 500.0f);
		ImGui::SliderFloat("Size y", &objData.size.y, 10.0f, 500.0f);
		
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");

		ImGui::Text("ENTER DATA FOR NEW LIGHT:");
		ImGui::Checkbox("Turned Off", &lightData.turnOff);
		ImGui::SliderInt("Number of rays", &lightData.noRays, 1, 300);
		ImGui::Text("light colour");
		ImGui::SliderFloat("R", &lightData.colour.r, 0.0f, 1.0f);
		ImGui::Text("");
		ImGui::SliderFloat("G", &lightData.colour.g, 0.0f, 1.0f);
		ImGui::Text("");
		ImGui::SliderFloat("B", &lightData.colour.b, 0.0f, 1.0f);
		
		ImGui::Text("");
		ImGui::Text("");

		if (ImGui::Button("Add Object")) {
			addOb = true;
		}
		else if (ImGui::Button("Add Light")) {
			addlightOb = true;
		}
		else if (ImGui::Button("Clear Scene")) {
			clearScene = true;
		}
		else {
			addOb = false;
			addlightOb = false;
			clearScene = false;
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Experiments")) {
		ImGui::Text("Carry out experiments here:");
		ImGui::Text("");

		if (ImGui::CollapsingHeader("Measuring angles with multiple objects")) {
			ImGui::Text("Align objects in a row and measure how the angle");
			ImGui::Text("of the light changes with different refractive indexes.");
			ImGui::Text("Enter refractive indexes for the objects:");
			ImGui::InputFloat("Object 1", &refractiveIndexes[0], 0.01f, 0.01f, 3);
			ImGui::InputFloat("Object 2", &refractiveIndexes[1], 0.01f, 0.01f, 3);
			ImGui::InputFloat("Object 3", &refractiveIndexes[2], 0.01f, 0.01f, 3);
			ImGui::InputFloat("Object 4", &refractiveIndexes[3], 0.01f, 0.01f, 3);
			ImGui::InputFloat("Object 5", &refractiveIndexes[4], 0.01f, 0.01f, 3);
			ImGui::InputFloat("Object 6", &refractiveIndexes[5], 0.01f, 0.01f, 3);

			ImGui::Text("");

			ImGui::InputInt("Object Gap", &objectGap);

			ImGui::Text("");

			ImGui::InputFloat("Incidence Angle", &incidenceAngle, 0.5f, 0.1f, 1);

			ImGui::Text("");

			if (ImGui::Button("Start")) {
				activeExperiment = 1;
			}
		}
		ImGui::Text("");

		if (ImGui::CollapsingHeader("Results")) {
			displayResults(angleArray);
		}

		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();
}

void guiManager::displayNumbers(std::vector<angleIndicator*> data) {
	std::string text;
	if (no_titlebar)		window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowBgAlpha(0.4f);
	for (unsigned int i = 0; i < data.size(); i++) {
		ImGui::Begin(std::to_string(i).c_str(), (bool*)0, window_flags);
		ImGui::SetWindowPos(ImVec2(data[i]->Position.x, data[i]->Position.y));
		ImGui::SetWindowSize(ImVec2(30.0f, 30.0f));
		ImGui::SetNextWindowBgAlpha(0.4f);
		text = std::to_string(i + 1);
		ImGui::Text(text.c_str());
		ImGui::End();
	}
}

void guiManager::displayResults(std::vector<angleIndicator*> data) {
	std::string text;
	//loop through all the angle indicators and read their attributes that contain the angles.
	for (unsigned int i = 0; i < data.size(); i++) {
		ImGui::Text("");
		//Output which angle the data is referring to
		text = "ANGLE " + std::to_string(i + 1);
		ImGui::Text(text.c_str());
		//Output angle of incidence
		text = "Angle 1: " + std::to_string(data[i]->iAngle);
		ImGui::Text(text.c_str());
		//Output angle of reflection/refraction.
		text = "Angle 2: " + std::to_string(data[i]->rAngle);
		ImGui::Text(text.c_str());
		ImGui::Text("");
	}
}