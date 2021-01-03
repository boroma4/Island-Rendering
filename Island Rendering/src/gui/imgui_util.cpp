#include "imgui_util.h"

#include <GLFW/glfw3.h>

void imgui_util::init(water_entity* water, island_entity* island, camera_entity* camera, GLFWwindow* window)
{
	this->fps = 0;
	this->water = water;
	this->island = island;
	this->camera = camera;
	this->window = window;
	//Setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
}

void imgui_util::render()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Menu");

	if (ImGui::CollapsingHeader("Water"))
	{
		ImGui::SliderFloat("Wave strength", &this->water->wave_strength, 0.0f, 0.5f);
		ImGui::SliderFloat("Wave speed", &this->water->wave_speed, 0.0f, 0.2f);
		ImGui::SliderFloat("Depth effect coef", &this->water->depth_effect_factor, 0.02f, 5.0f);
		ImGui::SliderFloat("Shininess", &this->water->shininess, 0.0f, 50.0f);
		ImGui::SliderFloat("Reflectivity", &this->water->reflectivity_power, 0.5f, 5.0f);
		ImGui::ColorEdit3("Base color", this->water->base_color);
	}
	
	if (ImGui::CollapsingHeader("Island"))
	{
		if (ImGui::Button("Regenerate"))
		{
			this->island->regenerate();
		}
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::SliderFloat("Render distance", &this->camera->far, 100.0f, 500.0f);
		ImGui::SliderFloat("FOV", &this->camera->projection_angle, 50.0f, 130.0f);
	}

	if (ImGui::CollapsingHeader("Help"))
	{
		ImGui::BulletText("Use WASD to move");
		ImGui::BulletText("Press M to switch between cursor/camera modes");
		ImGui::BulletText("Press F to pay.. switch fullscreen mode on/off");
		ImGui::BulletText("Use sections above to change the parameters as you like");
	}

	auto fps_text = "FPS: " + std::to_string(this->fps);
	ImGui::Text(fps_text.c_str());


	if (ImGui::Button("  Exit  "))
	{
		glfwSetWindowShouldClose(this->window, 1);
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_util::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
