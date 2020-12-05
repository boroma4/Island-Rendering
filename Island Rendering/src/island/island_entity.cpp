#include "island_entity.h"
#include "perlin_noise.h"
#include <iostream>
#include <GLFW\glfw3.h>
#include "../Settings.h"

void island_entity::init(shader_prog* shader)
{
	float w = 100.0f;
	float l = 100.0f;
	float h = 10.0f;

	this->shader = shader;
	heightMap = new HillGenerator(256, 180, 7, 20, 10, 30, 150, 123);
	position = glm::vec3(0.0, -h / 2, 0.0);
	rotation = glm::vec3(0.0, 1.0, 0.0);
	scaling = glm::vec3(w, h, l);

	transform = new entity_transform(position, scaling, rotation);

	heightMap->GenerateHeightmap();
	model = heightMap->exportMesh(1.0, 1.0);
}

void island_entity::draw(const float delta_time)
{
	shader->activate();

	// apply current transformation to the entity and send model matrix
	auto model_matrix = glm::mat4(1.0);	
	transform->apply(model_matrix);
	shader->uniformMatrix4fv("modelMatrix", model_matrix);

	model->Render();
}