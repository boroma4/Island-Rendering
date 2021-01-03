#pragma once
#include "../water/water_entity.h"
#include "../camera/camera_entity.h"
#include "../island/island_entity.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
class imgui_util
{
public:
	// add pointers to other objects to tune here
	void init(water_entity* w, island_entity* island, camera_entity* camera, GLFWwindow* window);
	void render();
	void destroy();

	int fps;
	
private:
	water_entity* water = nullptr;
	island_entity* island = nullptr;
	camera_entity* camera = nullptr;
	GLFWwindow* window = nullptr;
};

