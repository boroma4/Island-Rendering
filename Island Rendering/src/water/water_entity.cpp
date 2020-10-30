#include "water_entity.h"

#include <stack>

void water_entity::init(shader_prog* shader)
{
	this->shader = shader;
	VAO = create_quad(glm::vec3(0.447, 0.827, 0.933), shader);
}

void water_entity::draw()
{
	shader->activate();

    auto model_matrix = glm::mat4(1.0);
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 5.0, 0.0));
	model_matrix = glm::scale(model_matrix, glm::vec3(1.0, 1.0, 30.0));
    model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    shader->uniformMatrix4fv("modelMatrix", model_matrix);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
