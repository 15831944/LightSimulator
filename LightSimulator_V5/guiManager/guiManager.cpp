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

void guiManager::createObjectDetailsWindow(Object *&ob, lightObject *&lightOb, angleIndicator *&ind) {
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
	else if (ind != nullptr) {
		std::string a1, a2, ri1, ri2;

		a1 = "Incident angle: " + (std::to_string(*&ind->iAngle));
		a2 = "Reflected/Refracted angle: " + (std::to_string(*&ind->rAngle));
		ri1 = "Refractive index 1: " + (std::to_string(*&ind->n1));
		ri2 = "Refractive index 2: " + (std::to_string(*&ind->n2));

		ImGui::Text(a1.c_str());
		ImGui::Text(a2.c_str());
		ImGui::Text(ri1.c_str());
		ImGui::Text(ri2.c_str());
	}
	ImGui::End();
}

void guiManager::createSceneManagerWindow(bool &addOb, bool &addlightOb, Object *&ob, lightObject *&lightOb, angleIndicator *&ind) {
	ImGui::Begin("Scene Manager");
	ImGui::SetWindowPos(sceneManagerWindowPos);
	ImGui::SetWindowSize(sceneManagerWindowSize);

	ImGui::BeginTabBar("tabBar");
	if (ImGui::BeginTabItem("Object Info")) {
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
		else if (ind != nullptr) {
			std::string a1, a2, ri1, ri2;

			a1 = "Incident angle: " + (std::to_string(*&ind->iAngle));
			a2 = "Reflected/Refracted angle: " + (std::to_string(*&ind->rAngle));
			ri1 = "Refractive index 1: " + (std::to_string(*&ind->n1));
			ri2 = "Refractive index 2: " + (std::to_string(*&ind->n2));

			ImGui::Text(a1.c_str());
			ImGui::Text(a2.c_str());
			ImGui::Text(ri1.c_str());
			ImGui::Text(ri2.c_str());
		}
		else {
			ImGui::Text("No object selected!");
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Scene Settings")) {
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
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Experiments")) {
		ImGui::Text("Add experiments here that will display graphs");
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();
}