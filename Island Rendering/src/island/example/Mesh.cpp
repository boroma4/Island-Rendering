#include "Mesh.h"

Vertex::Vertex()
	:position(glm::vec3(0.0, 0.0, 0.0)), normal(glm::vec3(0.0, 0.0, 0.0))
{}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal)
	: position(position), normal(normal)
{}

Mesh::Mesh(Vertex* vertexBuffer, unsigned int vertexCount, GLenum primitive)
{
	m_isIndexed = false;
	bufferObjectSize = vertexCount;
	primitiveType = primitive;

	glGenBuffers(1, &bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertexBuffer, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)sizeof(glm::vec3));
}

Mesh::Mesh(std::vector<VertexComponents> format, void* vertexBuffer, unsigned int vertexCount, unsigned int* indexBuffer, unsigned int indexCount, GLenum primitive)
{
	m_isIndexed = true;
	bufferObjectSize = vertexCount;
	indexBufferSize = indexCount;
	primitiveType = primitive;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int totalSize = 0;
	for (int i = 0; i < format.size(); i++)
	{
		totalSize += format[i].size;
	}

	glGenBuffers(1, &bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
	glBufferData(GL_ARRAY_BUFFER, totalSize * vertexCount, vertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indexBuffer, GL_STATIC_DRAW);

	unsigned int offset = 0;
	for (unsigned int i = 0; i < format.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, format[i].dimensions, format[i].dataType, GL_FALSE, totalSize, (void*)offset);
		offset += format[i].size;
	}
}

void Mesh::Render()
{
	GLint currentVao;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);

	if (vao != currentVao)
		glBindVertexArray(vao);

	if (m_isIndexed)
		glDrawElements(primitiveType, indexBufferSize, GL_UNSIGNED_INT, NULL);
	else
		glDrawArrays(primitiveType, 0, bufferObjectSize);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &bufferObject);

	if (m_isIndexed)
		glDeleteBuffers(1, &indexBufferObject);
}