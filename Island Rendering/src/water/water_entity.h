#pragma once

#include "water_frame_buffers.h"
#include "../utils/geometry.h"
#include "../utils/shader_util.h"
#include "../Settings.h"

class water_entity
{
	
public:
	~water_entity();
	void init(shader_prog* shader);
	void draw();
	void bind_reflection_buffer();
	void bind_refraction_buffer();
	void unbind_current_buffer();

private:
	GLuint VAO;
	shader_prog* shader;
	water_frame_buffers* frame_buffers;
};
