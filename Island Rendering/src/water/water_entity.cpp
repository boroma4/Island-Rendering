#include "water_entity.h"

water_entity::~water_entity()
{
	this->shader->free();
	delete this->frame_buffers;
	delete this->transform;
}

void water_entity::init(shader_prog* shader)
{
	this->shader = shader;
	this->tile_vao = create_textured_quad(glm::vec3(base_color[0], base_color[1], base_color[2]), shader);
	this->frame_buffers = new water_frame_buffers();
	this->dudv_tex = load_texture(GL_TEXTURE_2D, GL_RGBA, "res/waterDUDV.png");
	this->normal_map_tex = load_texture(GL_TEXTURE_2D, GL_RGBA, "res/waterNormalMap.png");

	auto translation = glm::vec3(0.0f, WATER_LEVEL, 0.0f);
	auto scale = glm::vec3(4.0f, 1.0f, 10.0f);
	auto rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	this->transform = new entity_transform(translation, scale, rotation);

}

void water_entity::draw(const float delta_time)
{
	// update water distortion factor
	this->move_factor += this->wave_speed * delta_time;
	this->move_factor = std::fmod(this->move_factor, 1.0f);

	// send uniforms to the shader
	this->shader->activate();
	this->shader->uniform1f("moveFactor", this->move_factor);
	this->shader->uniform1f("waveStrength", this->wave_strength);
	this->shader->uniform1f("depthEffectFactor", this->depth_effect_factor);
	this->shader->uniform1f("shininess", this->shininess);
	this->shader->uniform1f("reflectivityPower", this->reflectivity_power);
	this->shader->uniformVec3("baseColor", glm::vec3(base_color[0], base_color[1], base_color[2]));
	
	this->shader->uniformTex2D("reflectionTexture", this->frame_buffers->reflection_texture);
	this->shader->uniformTex2D("depthTexture", this->frame_buffers->depth_texture);
	this->shader->uniformTex2D("refractionTexture", this->frame_buffers->refraction_texture);
	this->shader->uniformTex2D("dudvTexture", this->dudv_tex);
	this->shader->uniformTex2D("normalMapTexture", this->normal_map_tex);

	// apply current transformation to the entity and send model matrix
    auto model_matrix = glm::mat4(1.0);
    this->transform->apply(model_matrix);
    this->shader->uniformMatrix4fv("modelMatrix", model_matrix);
	
    glBindVertexArray(this->tile_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void water_entity::bind_reflection_buffer()
{
	this->frame_buffers->bind_reflection_fbo();
}

void water_entity::bind_refraction_buffer()
{
	this->frame_buffers->bind_refraction_fbo();
}

void water_entity::unbind_current_buffer()
{
	this->frame_buffers->unbind_current_fbo();
}
