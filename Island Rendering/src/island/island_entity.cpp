#include "island_entity.h"
#include "perlin_noise.h"
#include <iostream>
#include <GLFW\glfw3.h>
#include "../Settings.h"

void island_entity::init(shader_prog* shader)
{
	this->shader = shader;
	this->tile_vao = create_perlin_quad(glm::vec3(0.0, 0.0, 0.0), shader);

	//this->tile_vao = create_quad(glm::vec3(0.0, 0.0, 0.0), shader);

	this->transform = new entity_transform(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 0.0, 0.0));
}

void island_entity::draw(const float delta_time)
{
	this->shader->activate();

	// apply current transformation to the entity and send model matrix
	auto model_matrix = glm::mat4(1.0);
	
    //model_matrix = glm::rotate(model_matrix, glm::radians(90.0f) * float(glfwGetTime()) / 5.0f, glm::vec3(0.0, 1.0, 0.0));
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0, WATER_LEVEL - 10, 0.0));
	//model_matrix = glm::scale(model_matrix, glm::vec3(50.0, 0.0, 50.0));
	//model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	model_matrix = glm::translate(model_matrix, glm::vec3(-400, 0.0, -400));
	
	
	this->transform->apply(model_matrix);
	this->shader->uniformMatrix4fv("modelMatrix", model_matrix);

	glBindVertexArray(this->tile_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, this->mapSize * this->mapSize * 4);

	//unsigned int seed = 227;
	//PerlinNoise pn(seed);
	//int mapSize = 50;
	//float elementSize = 0.5;
	//float elementHeight = 4.0;

	//for (int i = 0; i < mapSize-1; i++) {
	//	for (int j = 0; j < mapSize-1; j++) {
	//		double x = (double)j / ((double)mapSize);
	//		double y = (double)i / ((double)mapSize);

	//		float noiseVar = 100.0;
	//		// Wood like structure
	//		double n = noiseVar * pn.noise(x, y, 0);
	//		n = n - floor(n);
	//		double n_next_x = noiseVar * pn.noise(x+1, y, 0);
	//		n_next_x = n_next_x - floor(n_next_x);
	//		double n_next_y = noiseVar * pn.noise(x, y+1, 0);
	//		n_next_y = n_next_y - floor(n_next_y);
	//		double n_next_xy = noiseVar * pn.noise(x+1, y+1, 0);
	//		n_next_xy = n_next_xy - floor(n_next_xy);

	//		//std::cout << n;

	//		// Map the values to the [0, 255] interval, for simplicity we use
	//		// tones of grey
	//		/*image.r[kk] = floor(255 * n);
	//		image.g[kk] = floor(255 * n);
	//		image.b[kk] = floor(255 * n);
	//		kk++;*/

	//		glBegin(GL_TRIANGLE_STRIP);
	//		//glColor3f(heights[i][j], heights[i][j], heights[i][j]);
	//		glColor3f(n, n, n);
	//		glVertex3f(i * elementSize, n * elementHeight, j * elementSize);
	//		glVertex3f((i + 1) * elementSize, n_next_x * elementHeight, j * elementSize);
	//		glVertex3f(i * elementSize, n_next_y * elementHeight, (j + 1) * elementSize);
	//		glVertex3f((i + 1) * elementSize, n_next_xy * elementHeight, (j + 1) * elementSize);
	//		glEnd();
	//	}
	//}
}