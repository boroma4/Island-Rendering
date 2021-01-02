#pragma once
#include "../utils/shader_util.h"
#include "../utils/geometry.h"
#include "../utils/entity_transform.h"
#include "island_generator.h"

class island_entity
{
	
public:
	void init(shader_prog* shader);
	void draw(const glm::mat4& view_matrix, const glm::mat4& projection_matrix, const glm::vec4& clipping_plane);
	entity_transform* transform;

private:
	GLuint tile_vao;
	shader_prog* shader;
	GLuint sand_tex;
	GLuint hills_tex;
	int mapSize = 50;
	int size;

	island_generator* heightMap;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaling;
};
