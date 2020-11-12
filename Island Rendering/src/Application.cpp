#define GLEW_STATIC    // to use static linking of GLEW library

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
#include "./island/island_entity.h"


struct app_stats
{
	double last_fps_check_time = 0.0, cur_time = 0.0, last_frame_time = 0.0, delta_time = 0.0;

	int frame_count = 0;

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

GLuint seafloor_vao, sky_vao; // just for testing

// --- Load the shaders declared in glsl files in the project folder ---//
shader_prog default_shader("shaders/default.vert.glsl", "shaders/default.frag.glsl");
shader_prog water_shader("shaders/water.vert.glsl", "shaders/water.frag.glsl");
shader_prog island_shader("shaders/island.vert.glsl", "shaders/island.frag.glsl");

// water object
water_entity water;
island_entity island;

// for window title
std::string app_name = "Island Rendering";

// for displaying FPS
app_stats statistics;

void init_scene() {
	// a nice tool for colors https://doc.instantreality.org/tools/color_calculator/
    water.init(&water_shader);
    island.init(&island_shader);
	seafloor_vao =  create_quad(glm::vec3(0.678, 0.674, 0.121), &default_shader);
	sky_vao = create_quad(glm::vec3(1, 0, 0), &default_shader);
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
	ms.push(ms.top());
		ms.top() = glm::translate(ms.top(), glm::vec3(0.0, WATER_LEVEL + 20, 0.0));
		ms.top() = glm::scale(ms.top(), glm::vec3(1.0, 2.0, 2.0));
	    ms.top() = glm::rotate(ms.top(), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	    shader->uniformMatrix4fv("modelMatrix", ms.top());
	    glBindVertexArray(sky_vao);
	    glDrawArrays(GL_TRIANGLES, 0, 6);
	ms.pop();
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
    glewExperimental = GL_TRUE;

    GLenum status = glewInit();
    if(status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(status));
    }

	std::cout << "Renderer: " << glGetString (GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString (GL_VERSION) << std::endl;
	
	// compile
    default_shader.use();
    water_shader.use();
    island_shader.use();

    //We initialize our stuff
    init_scene();

    float near = 1.0f;
    float far = 200.0f;

    glm::mat4 perspective = glm::perspective(glm::radians(80.0f), static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, near, far);
	glm::vec3 viewer = glm::vec3(30.0, 30.0, 30.0);
    glm::mat4 view = glm::lookAt(
        viewer, //Position
        glm::vec3(0.0, 0.0, 0.0),  //LookAt
        glm::vec3(0.0, 1.0, 0.0)   //Up
    );
	
	// if the sun is infinitely far
	glm::vec3 lightDirection (-0.0f, -1.0f, -0.0f);
	
    //Send the view and projection matrices to all shaders.
    default_shader.activate();
    default_shader.uniformMatrix4fv("projectionMatrix", perspective);
    default_shader.uniformMatrix4fv("viewMatrix", view);
    default_shader.uniformVec3("lightDirection", lightDirection);

    water_shader.activate();
    water_shader.uniformMatrix4fv("projectionMatrix", perspective);
    water_shader.uniformMatrix4fv("viewMatrix", view);
    water_shader.uniformVec3("lightDirection", lightDirection);	
    
    island_shader.activate();
    island_shader.uniformMatrix4fv("projectionMatrix", perspective);
    island_shader.uniformMatrix4fv("viewMatrix", view);
    island_shader.uniformVec3("lightDirection", lightDirection);

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
        default_shader.activate();

    	// draw everything aside the water into reflection buffer
    	default_shader.uniformVec4("clippingPlane",glm::vec4(0.0, 1.0, 0.0, -WATER_LEVEL)); // leave everything above water
    	water.bind_reflection_buffer();
        draw_scene(&default_shader);
        water.unbind_current_buffer();

		// draw everything aside the water into refraction buffer
    	water.bind_refraction_buffer();
		default_shader.uniformVec4("clippingPlane",glm::vec4(0.0, -1.0, 0.0, WATER_LEVEL)); // leave everything below water
        draw_scene(&default_shader);
        water.unbind_current_buffer(); // to enable drawing on the screen

    	// draw scene normally, reflection and refraction buffers will be used for water calculations
		default_shader.uniformVec4("clippingPlane",glm::vec4(0.0, 0.0, 0.0, 0.0));
    	draw_scene(&default_shader);

    	// finally draw the water
    	water.draw(statistics.delta_time);

        island.draw(statistics.delta_time);

    	// update time stamps and stuff
        statistics.on_frame();
    	// if a second has passed, update FPS in the window title    	
    	if (statistics.cur_time - statistics.last_fps_check_time >= 1.0)
		{
			auto new_title = app_name + " " + std::to_string(statistics.frame_count) + "FPS";
            glfwSetWindowTitle(win, new_title.c_str());
    		statistics.reset();
		}

    	// draw and check input events
    	glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
}
