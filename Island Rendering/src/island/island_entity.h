#pragma once
#include "../utils/shader_util.h"
#include "../utils/geometry.h"
#include "../utils/entity_transform.h"
#include "island_generator.h"

class island_entity
{
	
public:
	void init(shader_prog* shader);
	void draw(float delta_time);
	entity_transform* transform;

private:
	GLuint tile_vao;
	shader_prog* shader;
	int mapSize = 50;
	int size;

	island_generator* heightMap;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaling;
};
