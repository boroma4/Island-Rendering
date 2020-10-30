#pragma once

#include "../utils/geometry.h"
#include "../utils/shader_util.h"

class water_entity
{
	
public:
	void init(shader_prog* shader);
	void draw();

private:
	GLuint VAO;
	shader_prog* shader;
};
