#pragma once

#include "water_frame_buffers.h"
#include "../utils/geometry.h"
#include "../utils/shader_util.h"
#include "../Settings.h"
#include "../utils/texture_util.h"

class water_entity
{
	
public:
	~water_entity();
	void init(shader_prog* shader);
	void draw(float delta_time);
	void bind_reflection_buffer();
	void bind_refraction_buffer();
	void unbind_current_buffer();

private:
	GLuint tile_vao;
	GLuint dudv_tex;
	shader_prog* shader;
	water_frame_buffers* frame_buffers;
	const float WAVE_SPEED = 0.03f;
	float move_factor = 0;
};
