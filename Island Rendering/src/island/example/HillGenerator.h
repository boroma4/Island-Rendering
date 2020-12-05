#ifndef HILLGENERATOR_H
#define HILLGENERATOR_H
#include "Mesh.h"

class HillGenerator
{
	float** heightMap;
	float** bumpMap;

	unsigned int size;
	unsigned int numHills;
	unsigned int maxRadius;
	unsigned int minRadius;
	unsigned int islandRadius;
	unsigned int clampWidth;
	unsigned int seed;

	unsigned int maxHeight;
	unsigned int minHeight;

	void AddGaussianHill(bool isIsland);

	void AddSphereHill();

	void Normalize(float** map, float scale = 1.0f);

	void SumMaps();

	void ClampEdges();

	float RandomFloat(float min, float max);

	void PopulatePositions(Vertex** vertexGrid, float width, float height);

	void PopulateNormals(Vertex** vertexGrid);

	void PopulateColors(Vertex** vertexGrid);

	void CreateIndexArray(unsigned int* indexArray);

public:
	HillGenerator(unsigned int size,
		unsigned int numHills,
		unsigned int minRadius,
		unsigned int maxRadius,
		unsigned int minHeight,
		unsigned int maxHeight,
		unsigned int islandRadius,
		unsigned int seed);

	void GenerateHeightmap();

	unsigned int GetSize();

	Mesh* exportMesh(float width, float height);

	~HillGenerator();
};








#endif