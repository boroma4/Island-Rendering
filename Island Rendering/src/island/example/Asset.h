#ifndef OBJECT_H
#define OBJECT_H
//#include <glutil/glutil.h>
#include <glm/glm.hpp>
#include "Mesh.h"
//#include "Texture.h"
//#include "FrameBuffer.h"
#include "HillGenerator.h"
//#include "Timer.h"
#include <stack>
#include "../../utils/shader_util.h"

/*
struct Asset
{
	Scene* owner;

public:
	Asset();
	Asset(Scene* owner);

	virtual void Draw(glutil::MatrixStack modelMatrix);
};*/

class HillPlane
{
	Mesh* model;
	shader_prog* shader;
	//Texture* grassTexture;
	//Texture* sandTexture;
	HillGenerator HeightMap;

	glm::vec3 position;
	glm::vec3 rotationAxis;
	float rotationDegrees;
	glm::vec3 scaling;

public:
	HillPlane(float w, float l, float h, unsigned int seed, shader_prog* shader);

	void Draw(std::stack<glm::mat4> modelMatrix);
	//void DrawReflection(std::stack<glm::mat4> modelMatrix, float clipHeight);

	~HillPlane();
};

//class Skybox
//{
//	Mesh* model;
//	Texture* cubeMap;
//
//	glm::vec3 vertices[8] = {
//		glm::vec3(1.0, 1.0, 1.0),
//		glm::vec3(1.0, 1.0, -1.0),
//		glm::vec3(1.0, -1.0, -1.0),
//		glm::vec3(1.0, -1.0, 1.0),
//		glm::vec3(-1.0, 1.0, 1.0),
//		glm::vec3(-1.0, 1.0, -1.0),
//		glm::vec3(-1.0, -1.0, -1.0),
//		glm::vec3(-1.0, -1.0, 1.0)
//	};
//
//	unsigned int elements[36] = {
//		0, 3, 2,
//		0, 2, 1,
//		4, 7, 3,
//		4, 3, 0,
//		5, 6, 7,
//		5, 7, 4,
//		1, 2, 6,
//		1, 6, 5,
//		0, 1, 5,
//		0, 5, 4,
//		3, 7, 6,
//		3, 6, 2
//	};
//
//	float rotationDegrees;
//
//public:
//	Skybox(float rotateOffset);
//
//	void Draw(glutil::MatrixStack modelMatrix);
//
//	~Skybox();
//};
//
//class WaterPlane
//{
//private:
//	Mesh* model;
//
//	FrameBuffer* texBuffer;
//	Texture* reflection;
//	Texture* refractMask;
//	Texture* floorDepthTexture;
//	Texture* surfDepthTexture;
//	Texture* displacementTex;
//	Texture* normalMap;
//
//	glm::vec3 position;
//	glm::vec3 rotationAxis;
//	float rotationDegrees;
//	glm::vec3 scaling;
//
//	glm::mat4 clipToTexMat;
//
//	Timer texOffset;
//	Timer waveOffset;
//
//	Vertex vertices[4] = {
//		Vertex(glm::vec3(1.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)),
//		Vertex(glm::vec3(-1.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)),
//		Vertex(glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0)),
//		Vertex(glm::vec3(-1.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0))
//	};
//
//	unsigned int elements[6] = {
//		0, 1, 2,
//		1, 3, 2
//	};
//
//	void CopyDefaultBuffer();
//	void GenerateRefractMask();
//public:
//	WaterPlane(float scale, float height);
//
//	void Draw(glutil::MatrixStack modelMatrix);
//	void Reshape(int screenW, int screenH);
//	void UpdateLightingUniforms(glm::mat4 tanToWorld);
//};
#endif