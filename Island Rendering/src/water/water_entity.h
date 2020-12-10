#pragma once

#include "water_frame_buffers.h"
#include "../utils/geometry.h"
#include "../utils/shader_util.h"
#include "../Settings.h"
#include "../utils/texture_util.h"
#include "../utils/entity_transform.h"

class water_entity
{

public:
	entity_transform* transform;
	float wave_strength = 0.1f;
	float wave_speed = 0.03f;
	float depth_effect_factor = 3.0f;
	float shininess = 100.0f;
	float reflectivity_power = 3.0f;
	float base_color [3] = {0.090, 0.772, 0.909};

	
public:
	~water_entity();
	void init(shader_prog* shader);
	void draw(float delta_time, const glm::mat4& view_matrix, const glm::mat4& projection_matrix, const glm::vec3& camera_position);
	void bind_reflection_buffer();
	void bind_refraction_buffer();
	void unbind_current_buffer();
	void on_screen_resize(int w, int h);

private:
	GLuint tile_vao;
	GLuint dudv_tex;
	GLuint normal_map_tex;
	shader_prog* shader;
	water_frame_buffers* frame_buffers;
	float move_factor = 0;
};
