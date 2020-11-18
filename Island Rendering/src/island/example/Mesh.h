#ifndef MESH_H
#define MESH_H
//#include <glload/gl_3_3.h>
#include <glm/glm.hpp>
#include <vector>
#include <GLEW\glew.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;

	Vertex();
	Vertex(glm::vec3 position, glm::vec3 normal);
};

struct VertexComponents
{
	unsigned char dimensions;
	unsigned int size;
	GLenum dataType;

	VertexComponents(unsigned char dimensions, unsigned int size, GLenum dataType)
		:dimensions(dimensions), size(size), dataType(dataType)
	{}
};

class Mesh
{
	GLuint vao;
	GLuint bufferObject;
	unsigned int bufferObjectSize;
	GLuint indexBufferObject;
	unsigned int indexBufferSize;
	GLuint drawMode;
	bool m_isIndexed;
	GLenum primitiveType;

public:

	Mesh(Vertex* vertexBuffer, unsigned int vertexCount, GLenum primitive);

	Mesh(std::vector<VertexComponents> format, void* vertexBuffer, unsigned int vertexCount, unsigned int* indexBuffer, unsigned int indexCount, GLenum primitive);

	void Render();

	~Mesh();
};











#endif
