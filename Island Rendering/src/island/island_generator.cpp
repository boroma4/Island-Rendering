#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>
#include "island_generator.h"
#include <iostream>
#include <vector>

island_generator::island_generator(unsigned int size,
	unsigned int numHills,
	unsigned int minRadius,
	unsigned int maxRadius,
	unsigned int minHeight,
	unsigned int maxHeight,
	unsigned int islandRadius,
	unsigned int seed)
	:size(size), numHills(numHills), maxRadius(maxRadius), minRadius(minRadius), maxHeight(maxHeight), minHeight(minHeight), islandRadius(islandRadius), clampWidth(30), seed(seed)
{
	srand(seed);

	float* vertices = new float[size * size];
	memset((void*)vertices, NULL, size * size * sizeof(float));
	heightMap = new float* [size];

	for (int i = 0; i < size; i++)
	{
		heightMap[i] = vertices + size * i;
	}

	vertices = new float[size * size];
	memset((void*)vertices, NULL, size * size * sizeof(float));
	bumpMap = new float* [size];

	for (int i = 0; i < size; i++)
	{
		bumpMap[i] = vertices + size * i;
	}
}

float island_generator::RandomFloat(float min, float max)
{
	return min + (static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) * (max - min);
}

void island_generator::SumMaps()
{
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			heightMap[x][y] += bumpMap[x][y];
		}
	}
}

void island_generator::ClampEdges()
{
	for (int clampI = 0; clampI < clampWidth; clampI++)
	{
		float scaling = (cos((1 - clampI / (float)clampWidth) * 3.1415) + 1) * 0.5;

		for (int xI = clampI + 1; xI < size - clampI - 1; xI++)
		{
			heightMap[xI][clampI] *= scaling;
			heightMap[xI][size - clampI - 1] *= scaling;
		}
		for (int yI = clampI; yI < size - clampI; yI++)
		{
			heightMap[clampI][yI] *= scaling;
			heightMap[size - clampI - 1][yI] *= scaling;
		}
	}

}

void island_generator::GenerateHeightmap()
{
	for (int i = 0; i < numHills; i++)
	{
		AddGaussianHill(false);
	}

	for (int i = 0; i < size * 3; i++)
	{
		AddSphereHill();
	}

	float alpha = 0.75;
	Normalize(bumpMap, 1.0 - alpha);
	Normalize(heightMap, alpha);

	SumMaps();

	ClampEdges();
}


void island_generator::AddGaussianHill(bool isIsland)
{
	float originX;
	float originY;

	if (isIsland)
	{
		float theta = RandomFloat(0, 3.14159 * 2);

		float distance = RandomFloat(0, islandRadius);

		originX = size / 2.0f + cos(theta) * distance;
		originY = size / 2.0f + sin(theta) * distance;
	}
	else
	{
		originX = RandomFloat(0, size - 1);
		originY = RandomFloat(0, size - 1);
	}

	float hillRadius = RandomFloat(minRadius, maxRadius);
	float hillHeight = RandomFloat(minHeight, maxHeight);

	float hillLimit = hillRadius * 3;
	float heightSq = pow(hillHeight, 2);
	float expDenom = 2 * pow(hillRadius, 2);

	float minX = glm::clamp(originX - hillLimit + 1, 0.0f, (float)size - 1);
	float maxX = glm::clamp(originX + hillLimit, 0.0f, (float)size - 1);

	float minY = glm::clamp(originY - hillLimit + 1, 0.0f, (float)size - 1);
	float maxY = glm::clamp(originY + hillLimit, 0.0f, (float)size - 1);

	for (int x = minX; x <= maxX; x++)
	{
		float exponentX = (x - originX) * (x - originX);
		for (int y = minY; y <= maxY; y++)
		{
			float exponent = -(exponentX + (y - originY) * (y - originY)) / expDenom;
			heightMap[x][y] += heightSq * pow(2.71828, exponent);
		}
	}

}

void island_generator::AddSphereHill()
{
	float originX = RandomFloat(0, size - 1);
	float originY = RandomFloat(0, size - 1);

	float hillRadius = RandomFloat(minRadius, maxRadius);
	float sqHillRadius = pow(hillRadius, 2);

	float minX = glm::clamp(originX - hillRadius + 1, 0.0f, (float)size - 1);
	float maxX = glm::clamp(originX + hillRadius, 0.0f, (float)size - 1);

	float minY = glm::clamp(originY - hillRadius + 1, 0.0f, (float)size - 1);
	float maxY = glm::clamp(originY + hillRadius, 0.0f, (float)size - 1);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			float sqDist = pow(x - originX, 2) + pow(y - originY, 2);
			if (sqDist < sqHillRadius)
			{
				bumpMap[x][y] += sqHillRadius - sqDist;
			}
		}
	}

}

void island_generator::Normalize(float** map, float scale)
{
	float globalMax = map[0][0];
	float globalMin = map[0][0];

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			if (map[x][y] > globalMax)
				globalMax = map[x][y];
			else if (map[x][y] < globalMin)
				globalMin = map[x][y];
		}
	}

	if (globalMax - globalMin != 0)
	{
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				map[x][y] = scale * (map[x][y] - globalMin) / (float)(globalMax - globalMin);
			}
		}
	}
}

void island_generator::PopulatePositions(Vertex** vertexGrid, float width, float height)
{
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			vertexGrid[x][y].position.x = x * width / (float)(size - 1) - width / 2.0f;
			vertexGrid[x][y].position.y = heightMap[x][y];
			vertexGrid[x][y].position.z = y * height / (float)(size - 1) - height / 2.0f;
		}
	}
}

void island_generator::PopulateNormals(Vertex** vertexGrid)
{
	int x0, x1;
	int y0, y1;

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			x0 = x - 1;
			x1 = x + 1;
			if (x == 0)
				x0 = x;
			else if (x == size - 1)
				x1 = x;

			glm::vec3 b = vertexGrid[x1][y].position - vertexGrid[x0][y].position;

			y0 = y - 1;
			y1 = y + 1;
			if (y == 0)
				y0 = y;
			else if (y == size - 1)
				y1 = y;

			glm::vec3 a = vertexGrid[x][y1].position - vertexGrid[x][y0].position;


			vertexGrid[x][y].normal = glm::normalize(glm::cross(a, b));
		}
	}
}

void island_generator::PopulateColors(Vertex** vertexGrid)
{

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			float height = heightMap[x][y];
			//std::cout << norm;
			//vertexGrid[x][y].color = glm::vec3{ norm,0,0 };
			vertexGrid[x][y].color = glm::vec3{ height, height, height };
		}
	}
}

void island_generator::CreateIndexArray(unsigned int* indexArray)
{
	int index = 0;

	for (unsigned int x = 0; x < size - 1; x++)
	{
		indexArray[index++] = x * size;

		for (unsigned int y = 0; y < size; y++)
		{
			indexArray[index++] = y + x * size;
			indexArray[index++] = y + (x + 1) * size;
		}

		indexArray[index++] = size - 1 + (x + 1) * size;
	}
}

GLuint island_generator::getVao(float width, float height)
{
	Vertex* vertexArray = new Vertex[size * size];
	Vertex** vertexGrid = new Vertex * [size];
	for (int i = 0; i < size; i++)
	{
		vertexGrid[i] = vertexArray + i * size;
	}

	PopulatePositions(vertexGrid, width, height);

	PopulateColors(vertexGrid);

	PopulateNormals(vertexGrid);

	// no. indices required for GL_TRIANGLE_STRIP + 4 degenerative triangles per strip
	unsigned int indexSize = (size - 1) * (size * 2) + (size - 2) * 2;

	unsigned int* indexArray = new unsigned int[indexSize];
	CreateIndexArray(indexArray);

	std::vector<VertexComponents> format;
	format.push_back(VertexComponents(3, sizeof(glm::vec3), GL_FLOAT));
	format.push_back(VertexComponents(3, sizeof(glm::vec3), GL_FLOAT));
	format.push_back(VertexComponents(3, sizeof(glm::vec3), GL_FLOAT));

	return create_handle(format, (void*)vertexArray, size * size, indexArray, indexSize, GL_TRIANGLE_STRIP);

	delete[] vertexArray;
	delete[] indexArray;
}

island_generator::~island_generator()
{
	delete[] heightMap;
	delete[] bumpMap;
}