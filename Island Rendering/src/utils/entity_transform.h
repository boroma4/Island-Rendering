#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class entity_transform
{
public:
	glm::vec3 translation;
	glm::vec3 scale;
	glm::vec3 rotation;

	entity_transform(): translation(0), scale(0), rotation(0){}
	
	entity_transform(glm::vec3 t, glm::vec3 s, glm::vec3 r): translation(t), scale(s), rotation(r){}

	void apply(glm::mat4& model_matrix)
	{
		model_matrix = glm::translate(model_matrix, translation);
		model_matrix = glm::scale(model_matrix, scale);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1.0, 0, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0.0, 0, 1.0));
	}
};
