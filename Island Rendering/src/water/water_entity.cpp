#include "water_entity.h"

water_entity::~water_entity()
{
	shader->free();
	delete this->frame_buffers;
}

void water_entity::init(shader_prog* shader)
{
	this->shader = shader;
	this->VAO = create_quad(glm::vec3(0.447, 0.827, 0.933), shader);
	this->frame_buffers = new water_frame_buffers();
}

void water_entity::draw()
{
	shader->activate();

    auto model_matrix = glm::mat4(1.0);
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0, WATER_LEVEL, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(2.0, 1.0, 30.0));
    model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    shader->uniformMatrix4fv("modelMatrix", model_matrix);
    glBindVertexArray(this->VAO);
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
