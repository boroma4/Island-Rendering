#define STB_IMAGE_IMPLEMENTATION  // to use image loading (for icon)

// ---------------------------- Includes -------------------------- //
#include <chrono>
#include <stdlib.h>         // C++ standard library
#include <stdio.h>          // Input/Output
#include <GLEW/glew.h>      // OpenGL Extension Wrangler -
#include <GLFW/glfw3.h>     // Windows and input
#include <glm/glm.hpp>      // OpenGL math library
#include <iostream>         // Input and Output
#include <stack>            // Stack
#include <thread>
#include "Settings.h"
#include "./utils/shader_util.h"
#include "./utils/geometry.h"
#include "./water/water_entity.h"
#include "./island/island_entity.h"
#include "./skybox/skybox_entity.h"
#include <time.h>
#include "./camera/camera_entity.h"
#include "./vendor/stb_image.h"
#include "./vendor/imgui/imgui.h"
#include "./gui/imgui_util.h"


struct app_stats
{
	double last_fps_check_time = 0.0, cur_time = 0.0, last_frame_time = 0.0, delta_time = 0.0;
	double cursor_last_x = SCREEN_WIDTH / 2.0f, cursor_last_y = SCREEN_HEIGHT / 2.0f;
	int frame_count = 0;
	bool is_first_mouse_move = true, is_borderless = false, is_camera_movement_allowed = false;
	int last_window_width, last_window_height, last_window_x_pos, last_window_y_pos;

	void on_frame()
	{
		cur_time = glfwGetTime();
		delta_time = cur_time - last_frame_time;
		last_frame_time = cur_time;
		frame_count++;
	}
	
	void reset()
	{
		frame_count = 0;
		last_fps_check_time = cur_time;
	}
};

// to enable NVIDIA GPU
extern "C"
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// --- Load the shaders declared in glsl files in the project folder ---//
//shader_prog default_shader("shaders/default.vert.glsl", "shaders/default.frag.glsl");
shader_prog water_shader("shaders/water.vert.glsl", "shaders/water.frag.glsl");
shader_prog island_shader("shaders/island.vert.glsl", "shaders/island.frag.glsl");
shader_prog skybox_shader("shaders/skybox.vert.glsl", "shaders/skybox.frag.glsl");

imgui_util gui;
// water object
water_entity water;
island_entity island;

// skybox object
skybox_entity skybox;

// camera
camera_entity camera(glm::vec3(30.0f, 30.0f, 30.0f));

// for window title
std::string app_name = "Island Rendering";

// for displaying FPS
app_stats statistics;

void init_scene() {
	// a nice tool for colors https://doc.instantreality.org/tools/color_calculator/
	water.init(&water_shader);
	skybox.init(&skybox_shader);
    island.init(&island_shader);
}

void update_window_title(GLFWwindow* window)
{
	// if a second has passed, update FPS in the window title    	
    if (statistics.cur_time - statistics.last_fps_check_time >= 1.0)
	{
		auto new_title = app_name + " " + std::to_string(statistics.frame_count) + "FPS";
        glfwSetWindowTitle(window, new_title.c_str());
    	gui.fps = statistics.frame_count;
    	statistics.reset();
	}
}

void load_window_icon(GLFWwindow* window)
{
	GLFWimage images[1]; 
	images[0].pixels = stbi_load("res/icon.png", &images[0].width, &images[0].height, nullptr, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images); 
	stbi_image_free(images[0].pixels);
}


void toggle_borderless_mode(GLFWwindow* window)
{
	// if turning off borderless mode, set the window with last parameters
	if(statistics.is_borderless)
	{
		glfwSetWindowMonitor(window, nullptr, statistics.last_window_x_pos, statistics.last_window_y_pos, statistics.last_window_width, statistics.last_window_height, 0);
	}
	// if turning on, save parameters and maximize the window
    else
    {
	    glfwGetWindowSize(window, &statistics.last_window_width, &statistics.last_window_height);
    	glfwGetWindowPos(window, &statistics.last_window_x_pos, &statistics.last_window_y_pos);
    	auto* monitor = glfwGetPrimaryMonitor();
		const auto* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
    }
	glfwSwapInterval(1);
	statistics.is_borderless = !statistics.is_borderless;
}

void toggle_mouse_mode(GLFWwindow* window)
{
	if(statistics.is_camera_movement_allowed)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
    else
    {
	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
	statistics.is_camera_movement_allowed = !statistics.is_camera_movement_allowed;
}


/*
 * Could create some input util later
 */

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, statistics.delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, statistics.delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, statistics.delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, statistics.delta_time);
}

// it's actually better for 1-time presses
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        toggle_borderless_mode(window);
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
        toggle_mouse_mode(window);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{	
    if (statistics.is_first_mouse_move)
    {
        statistics.cursor_last_x = xpos;
        statistics.cursor_last_y = ypos;
        statistics.is_first_mouse_move = false;
    }

    float xoffset = xpos - statistics.cursor_last_x;
    float yoffset = statistics.cursor_last_y - ypos; // reversed since y-coordinates go from bottom to top

     statistics.cursor_last_x = xpos;
     statistics.cursor_last_y = ypos;

	if(statistics.is_camera_movement_allowed)
	{
		camera.process_mouse_movement(xoffset, yoffset);
	}
}

void window_size_callback(GLFWwindow* window, int w, int h)
{
	camera.process_screen_resize(w, h);
 	water.on_screen_resize(w, h);
}

bool should_not_render(GLFWwindow* window)
{
	auto focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
	return !focused && statistics.is_borderless;
}

// ---------------------------- Main -------------------------- //
int main(int argc, char *argv[]) {
	
    if (!glfwInit()) {
        exit (EXIT_FAILURE);
    }

    auto* win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, app_name.c_str(), nullptr, nullptr);
    glewInit();

    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(win);
	glfwSwapInterval(1);
    glewExperimental = GL_TRUE;

    auto status = glewInit();
    if(status != GLEW_OK) std::cout << "Error: " << glewGetErrorString(status) << std::endl;
	std::cout << "Renderer: " << glGetString (GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString (GL_VERSION) << std::endl;

    // put a cute icon next to the title
	load_window_icon(win);

    // inputs
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);

	// compile shaders
    //default_shader.use();
    water_shader.use();
	skybox_shader.use();
    island_shader.use();

    //Init
    init_scene();

    gui.init(&water, win);
	glm::vec3 lightDirection (-0.0f, -1.0f, -0.0f);
	
    //Send the view and projection matrices to all shaders.
    //default_shader.activate();
    //default_shader.uniformMatrix4fv("projectionMatrix", perspective);
    //default_shader.uniformMatrix4fv("viewMatrix", camera.get_view_matrix());
    //default_shader.uniformVec3("lightDirection", lightDirection);

    water_shader.activate();
    water_shader.uniformVec3("lightDirection", lightDirection);
	water_shader.uniformVec2("frustum", camera.get_frustum());
    
    island_shader.activate();
    island_shader.uniformVec3("lightDirection", lightDirection);

	// OpenGL settings
	glEnable(GL_CLIP_DISTANCE0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	statistics.last_fps_check_time = glfwGetTime();

    while (!glfwWindowShouldClose(win)) {

    	// basically, if you alt-tab in borderless fullscreen, the FPS gets crazy (haven't found a reason yet)
    	// to prevent this I added some artificial delay to frames in such cases
    	if (should_not_render(win))
    	{
    		std::this_thread::sleep_for(std::chrono::milliseconds(10));
    	}
    	
    	processInput(win);      //input processing will be called every frame

    	// get those once and reuse 
    	auto projection = camera.get_projection_matrix();
    	auto view = camera.get_view_matrix();
    	/*
    	 * 1) Enable clipping
    	 * 2) Save clipped land and sky to reflection and refraction buffers
    	 * 3) Disable clipping and draw the scene
    	 */
    	
    	// NB! Skybox doesn't need to be clipped, only terrain does

    	/*
    	 * STEP 1
    	 * -------
    	 * Draw everything aside the water into reflection buffer   
    	 */
    	water.bind_reflection_buffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	auto distance = 2 * (camera.position.y - WATER_LEVEL);
        // move the camera under the water
    	camera.position.y -= distance;
        camera.invert_pitch();
    	
        skybox.draw(projection, camera.get_view_matrix());
    	// only draw island parts located above the water
    	island.draw(camera.get_view_matrix(), projection, glm::vec4(0.0, 1.0, 0.0, -WATER_LEVEL));

        // reverse the changes
		camera.position.y += distance;
        camera.invert_pitch();
        
        water.unbind_current_buffer();
    	
        /*
    	 * STEP 2
    	 * -------
    	 * Draw everything aside the water into refraction buffer
    	 */
	
    	water.bind_refraction_buffer();
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.draw(projection, view);

    	// only draw island parts located below the water
    	island.draw(view, projection, glm::vec4(0.0, -1.0, 0.0, WATER_LEVEL + 1));
        water.unbind_current_buffer();

    	
        /*
    	 * STEP 3
    	 * --------
    	 * Disable clipping, draw scene normally, reflection and refraction buffers will be used for water calculations
    	 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
        skybox.draw(projection, view);
        island.draw(view, projection, glm::vec4(0.0, 0.0, 0.0, 0.0));

    	// finally draw the water
    	water.draw(statistics.delta_time, view, projection, camera.position);

    	// render gui menu window
    	gui.render();
    	// update time stamps and stuff
        statistics.on_frame();

    	// set the fps in the title
    	update_window_title(win);

    	// draw and check input events
    	glfwSwapBuffers(win);
        glfwPollEvents();
    }

	gui.destroy();
	glfwTerminate();
	glfwDestroyWindow(win);
}
