#define GLEW_STATIC    // to use static linking of GLEW library
#define STB_IMAGE_IMPLEMENTATION  // to use image loading (for icon)

// ---------------------------- Includes -------------------------- //
#include <stdlib.h>         // C++ standard library
#include <stdio.h>          // Input/Output
#include <GLEW/glew.h>      // OpenGL Extension Wrangler -
#include <GLFW/glfw3.h>     // Windows and input
#include <glm/glm.hpp>      // OpenGL math library
#include <iostream>         // Input and Output
#include <stack>            // Stack
#include "Settings.h"
#include "./utils/shader_util.h"
#include "./utils/geometry.h"
#include "./water/water_entity.h"
#include "./skybox/skybox_entity.h"
#include "./camera/camera_entity.h"
#include "./vendor/stb_image.h"

struct app_stats
{
	double last_fps_check_time = 0.0, cur_time = 0.0, last_frame_time = 0.0, delta_time = 0.0;
	double cursor_last_x = SCREEN_WIDTH / 2.0f, cursor_last_y = SCREEN_HEIGHT / 2.0f;
	int frame_count = 0;
	bool is_first_mouse_move = true;

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

GLuint seafloor_vao; // just for testing
bool is_camera_movement_allowed = false;

// --- Load the shaders declared in glsl files in the project folder ---//
shader_prog default_shader("shaders/default.vert.glsl", "shaders/default.frag.glsl");
shader_prog water_shader("shaders/water.vert.glsl", "shaders/water.frag.glsl");
shader_prog skybox_shader("shaders/skybox.vert.glsl", "shaders/skybox.frag.glsl");

// water object
water_entity water; 

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
	seafloor_vao =  create_quad(glm::vec3(0.678, 0.674, 0.121), &default_shader);
}

//basic stuff for testing
void draw_scene(shader_prog* shader) {
    shader->activate();

    std::stack<glm::mat4> ms;
	ms.push(glm::mat4(1.0));

	ms.push(ms.top());
		ms.top() = glm::translate(ms.top(), glm::vec3(0.0, WATER_LEVEL - 10, 0.0));
		ms.top() = glm::scale(ms.top(), glm::vec3(20.0, 5.0, 30.0));
	    ms.top() = glm::rotate(ms.top(), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	    shader->uniformMatrix4fv("modelMatrix", ms.top());
	    glBindVertexArray(seafloor_vao);
	    glDrawArrays(GL_TRIANGLES, 0, 6);
	ms.pop();
}

void update_window_title(GLFWwindow* window)
{
	// if a second has passed, update FPS in the window title    	
    if (statistics.cur_time - statistics.last_fps_check_time >= 1.0)
	{
		auto new_title = app_name + " " + std::to_string(statistics.frame_count) + "FPS";
        glfwSetWindowTitle(window, new_title.c_str());
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

/*
 * Could create some input util later
 */

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT))
        glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        camera.process_keyboard(FORWARD, statistics.delta_time);
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        camera.process_keyboard(BACKWARD, statistics.delta_time);
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        camera.process_keyboard(LEFT, statistics.delta_time);
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        camera.process_keyboard(RIGHT, statistics.delta_time);
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        skybox.changeSkybox(-1);
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        skybox.changeSkybox(1);
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

	if(is_camera_movement_allowed)
	{
		camera.process_mouse_movement(xoffset, yoffset);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
	   is_camera_movement_allowed = true;
	   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
	 if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	 {
		is_camera_movement_allowed = false;
	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	 }
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
    if(status != GLEW_OK) {
    	std::cout << "Error: " << glewGetErrorString(status) << std::endl;
    }

	std::cout << "Renderer: " << glGetString (GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString (GL_VERSION) << std::endl;

    // put a cute icon next to the title
	load_window_icon(win);

    // inputs
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetKeyCallback(win, key_callback);

	// compile shaders
    default_shader.use();
	water_shader.use();
	skybox_shader.use();

    //Init
    init_scene();

	/*
	 * Need to move this stuff somewhere at some point
	 */
    float near = 1.0f;
    float far = 200.0f;

    glm::mat4 perspective = glm::perspective(glm::radians(80.0f), static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, near, far);
	glm::vec3 lightDirection (-0.0f, -1.0f, -0.0f);
	
    //Send the view and projection matrices to all shaders.
    default_shader.activate();
    default_shader.uniformMatrix4fv("projectionMatrix", perspective);
    default_shader.uniformMatrix4fv("viewMatrix", camera.get_view_matrix());
    default_shader.uniformVec3("lightDirection", lightDirection);

	water_shader.activate();
    water_shader.uniformMatrix4fv("projectionMatrix", perspective);
    water_shader.uniformMatrix4fv("viewMatrix", camera.get_view_matrix());
    water_shader.uniformVec3("lightDirection", lightDirection);

    skybox_shader.activate();
    skybox_shader.uniformMatrix4fv("projectionMatrix", perspective);
    skybox_shader.uniformMatrix4fv("viewMatrix", camera.get_view_matrix());

	// OpenGL settings
	glEnable(GL_CLIP_DISTANCE0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	
	statistics.last_fps_check_time = glfwGetTime();

    while (!glfwWindowShouldClose(win)) {
    	
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    	default_shader.activate();
    	default_shader.uniformVec4("clippingPlane",glm::vec4(0.0, 1.0, 0.0, -WATER_LEVEL)); // leave everything above water

    	auto distance = 2 * (camera.position.y - WATER_LEVEL);
        // move the camera under the water
    	camera.position.y -= distance;
        camera.invert_pitch();

    	default_shader.activate();
    	default_shader.uniformMatrix4fv("viewMatrix", camera.get_view_matrix());
    		
	    skybox.draw(perspective, camera.get_view_matrix());
	    draw_scene(&default_shader);

       // reverse the changes
		camera.position.y += distance;
        camera.invert_pitch();
        	
    	default_shader.activate();
    	default_shader.uniformMatrix4fv("viewMatrix", camera.get_view_matrix());
        
        water.unbind_current_buffer();
    	
        /*
    	 * STEP 2
    	 * -------
    	 * Draw everything aside the water into refraction buffer
    	 */
	
    	water.bind_refraction_buffer();

    	default_shader.activate();
		default_shader.uniformVec4("clippingPlane",glm::vec4(0.0, -1.0, 0.0, WATER_LEVEL)); // leave everything below water
    	
        skybox.draw(perspective, camera.get_view_matrix());
        draw_scene(&default_shader);
    	
        water.unbind_current_buffer();
    	
        /*
    	 * STEP 3
    	 * --------
    	 * Disable clipping, draw scene normally, reflection and refraction buffers will be used for water calculations
    	 */
    
    	default_shader.activate();
		default_shader.uniformVec4("clippingPlane",glm::vec4(0.0, 0.0, 0.0, 0.0));
    	
    	draw_scene(&default_shader);
        skybox.draw(perspective, camera.get_view_matrix());

    	// finally draw the water
    	water_shader.activate();
    	water_shader.uniformMatrix4fv("viewMatrix", camera.get_view_matrix());
    	water_shader.uniformVec3("cameraPosition", camera.position);
    	water.draw(statistics.delta_time);

    	// update time stamps and stuff
        statistics.on_frame();

    	// set the fps in the title
    	update_window_title(win);

    	// draw and check input events
    	glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
}
