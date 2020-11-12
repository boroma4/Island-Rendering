#pragma once
#include "../utils/shader_util.h"
#include "../utils/geometry.h"
#include "../utils/entity_transform.h"

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
};
