#include "water_entity.h"

water_entity::~water_entity()
{
	shader->free();
	delete this->frame_buffers;
}

void water_entity::init(shader_prog* shader)
{
	this->shader = shader;
	this->tile_vao = create_textured_quad(glm::vec3(0.090, 0.772, 0.909), shader);
	this->frame_buffers = new water_frame_buffers();
	this->dudv_tex = load_texture(GL_TEXTURE_2D, GL_RGBA, "res/waterDUDV.png");
}

void water_entity::draw(const float delta_time)
{
	this->move_factor += this->WAVE_SPEED * delta_time;
	this->move_factor = std::fmod(this->move_factor, 1.0f);
	
	this->shader->activate();
	this->shader->uniform1f("moveFactor", this->move_factor);
	this->shader->uniformTex2D("reflectionTexture", this->frame_buffers->reflection_texture);
	this->shader->uniformTex2D("refractionTexture", this->frame_buffers->refraction_texture);
	this->shader->uniformTex2D("dudvTexture", this->dudv_tex);
	
    auto model_matrix = glm::mat4(1.0);
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0, WATER_LEVEL, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(2.0, 1.0, 20.0));
    model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    shader->uniformMatrix4fv("modelMatrix", model_matrix);
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
