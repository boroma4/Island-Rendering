#include <GLEW\glew.h>
#ifndef ISLANDGENERATOR_H
#define ISLANDGENERATOR_H

#include "../utils/geometry.h"

class island_generator
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
	bool isIsland;

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
	island_generator(unsigned int size,
		unsigned int numHills,
		unsigned int minRadius,
		unsigned int maxRadius,
		unsigned int minHeight,
		unsigned int maxHeight,
		unsigned int islandRadius,
		bool isIsland,
		unsigned int seed);

	void GenerateHeightmap();

	unsigned int GetSize();

	GLuint getVao(float width, float height);

	~island_generator();
};








#endif