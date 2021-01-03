#include "island_entity.h"
#include <iostream>
#include <GLFW\glfw3.h>
#include "../Settings.h"
#include <time.h>
#include "../utils/texture_util.h"

void island_entity::init(shader_prog* shader)
{
	float w = 200.0f;
	float l = 200.0f;
	float h = 30.0f;

	this->shader = shader;
	this->sand_tex = load_texture(GL_TEXTURE_2D, GL_RGBA, "res/sand.jpg");
	this->hills_tex = load_texture(GL_TEXTURE_2D, GL_RGBA, "res/hills.jpg");
	size = 256;
	time_t ticks;
	time(&ticks);
	heightMap = new island_generator(size, 180, 7, 20, 10, 40, 150, true, (unsigned int)ticks);
	position = glm::vec3(0.0, WATER_LEVEL - 7, 0.0);
	rotation = glm::vec3(0.0, 1.0, 0.0);
	scaling = glm::vec3(w, h, l);

	transform = new entity_transform(position, scaling, rotation);

	heightMap->GenerateHeightmap();
	tile_vao = heightMap->getVao(1.0, 1.0);
}

void island_entity::draw(const glm::mat4& view_matrix, const glm::mat4& projection_matrix, const glm::vec4& clipping_plane)
{
	shader->activate();

	// apply current transformation to the entity and send model matrix
	auto model_matrix = glm::mat4(1.0);	
	transform->apply(model_matrix);
	shader->uniformMatrix4fv("modelMatrix", model_matrix);
	shader->uniformMatrix4fv("viewMatrix", view_matrix);
	shader->uniformMatrix4fv("projectionMatrix", projection_matrix);
	shader->uniformVec4("clippingPlane",clipping_plane);

	shader->uniformTex2D("sandTexture", this->sand_tex);
	shader->uniformTex2D("hillsTexture", this->hills_tex);
	
	glBindVertexArray(this->tile_vao);
	glDrawElements(GL_TRIANGLE_STRIP, (size - 1) * (size * 2) + (size - 2) * 2, GL_UNSIGNED_INT, NULL);
}

void island_entity::regenerate()
{
	time_t ticks;
	time(&ticks);
	delete heightMap;
	heightMap = new island_generator(size, 180, 7, 20, 10, 40, 150, true, (unsigned int)ticks);
	heightMap->GenerateHeightmap();
	tile_vao = heightMap->getVao(1.0, 1.0);
}
