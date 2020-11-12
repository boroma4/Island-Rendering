#include "geometry.h"
#include "../island/perlin_noise.h"
#include <iostream>

/**
 * This function creates a colored quad.
 * These quads have a position, color and normal for each vertex.
 * Used for hangar walls.
 */
GLuint create_quad(glm::vec3 color, shader_prog* shader) {
    GLuint vertexArrayHandle;
    GLuint arrayBufferHandle;

    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);
    glGenBuffers(1, &arrayBufferHandle);

    float s = 10.0;

    GLfloat vertexData[] = {
        //  X Y  Z              Color                 Normal      
        -s,-s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 
         s,-s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
        -s, s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 
         s,-s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
         s, s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
        -s, s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLuint loc = glGetAttribLocation(shader->getProg(), "position");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "position"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (const GLvoid*)(0*sizeof(float)));

    loc = glGetAttribLocation(shader->getProg(), "color");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "color"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (const GLvoid*)(3*sizeof(float)));

    loc = glGetAttribLocation(shader->getProg(), "normal");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "normal"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (const GLvoid*)(6*sizeof(float)));

    glBindVertexArray(0);

    return vertexArrayHandle;
}

float* CrossProduct(float* a, float* b)
{
    float Product[3];

    //Cross product formula 
    Product[0] = (a[1] * b[2]) - (a[2] * b[1]);
    Product[1] = (a[2] * b[0]) - (a[0] * b[2]);
    Product[2] = (a[0] * b[1]) - (a[1] * b[0]);

    return Product;
}


/**
 * This function creates a colored perlin quad.
 */
GLuint create_perlin_quad(glm::vec3 color, shader_prog* shader) {
    GLuint vertexArrayHandle;
    GLuint arrayBufferHandle;

    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);
    glGenBuffers(1, &arrayBufferHandle);

    float s = 10.0;

    int mapSize = 50;
    float elementSize = 1.5;
    float elementHeight = 4.0;

    unsigned int seed = 227;
    PerlinNoise pn(seed);

    

    std::vector<float> tempArray = { };

    for (int i = 0; i < mapSize - 1; i++) {
        for (int j = 0; j < mapSize - 1; j++) {
            double x = (double)j / ((double)mapSize);
            double y = (double)i / ((double)mapSize);

            float noiseVar = 100.0;
            // Wood like structure
            float n = noiseVar * pn.noise(x, y, 0);
            n = n - floor(n);
            float n_next_x = noiseVar * pn.noise(x + 1, y, 0);
            n_next_x = n_next_x - floor(n_next_x);
            float n_next_y = noiseVar * pn.noise(x, y + 1, 0);
            n_next_y = n_next_y - floor(n_next_y);
            float n_next_xy = noiseVar * pn.noise(x + 1, y + 1, 0);
            n_next_xy = n_next_xy - floor(n_next_xy);

            float vertex1[] = { i * elementSize, n * elementHeight, j * elementSize };
            float vertex2[] = { (i + 1) * elementSize, n_next_x * elementHeight, j * elementSize };
            float vertex3[] = { i * elementSize, n_next_y * elementHeight, (j + 1) * elementSize };
            float vertex4[] = { (i + 1) * elementSize, n_next_xy * elementHeight, (j + 1) * elementSize };

            float Vector[3], Vector2[3];

            Vector[0] = vertex3[0] - vertex1[0];
            Vector[1] = vertex3[1] - vertex1[1];
            Vector[2] = vertex3[2] - vertex1[2];

            Vector2[0] = vertex2[0] - vertex1[0];
            Vector2[1] = vertex2[1] - vertex1[1];
            Vector2[2] = vertex2[2] - vertex1[2];

            float* Normal = CrossProduct(Vector, Vector2);

            tempArray.insert(tempArray.end(), {
                //X Y  Z                               Color        Normal      
                vertex1[0], vertex1[1], vertex1[2],  n, n, n, Normal[0], Normal[1], Normal[2],
                vertex2[0], vertex2[1], vertex2[2],  n, n, n, Normal[0], Normal[1], Normal[2],
                vertex3[0], vertex3[1], vertex3[2],  n, n, n, Normal[0], Normal[1], Normal[2],
                vertex4[0], vertex4[1], vertex4[2],  n, n, n, Normal[0], Normal[1], Normal[2],
            });
        }
    }
    std::cout << "0. size: " << tempArray.size() << '\n';
    GLfloat vertexData[50 * 50 * 4 * 9] = {};
    std::copy(tempArray.begin(), tempArray.end(), vertexData);

    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLuint loc = glGetAttribLocation(shader->getProg(), "position");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "position"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const GLvoid*)(0 * sizeof(float)));

    loc = glGetAttribLocation(shader->getProg(), "color");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "color"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

    loc = glGetAttribLocation(shader->getProg(), "normal");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "normal"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const GLvoid*)(6 * sizeof(float)));

    glBindVertexArray(0);

    return vertexArrayHandle;
}

GLuint create_textured_quad(glm::vec3 color, shader_prog* shader) {
    GLuint vertexArrayHandle;
    GLuint arrayBufferHandle;

    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);
    glGenBuffers(1, &arrayBufferHandle);

    float s = 10.0;

    GLfloat vertexData[] = {
        //  X Y  Z              Color                 Normal         U    V
        -s,-s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         s,-s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -s, s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         s,-s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         s, s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -s, s, 0.0f,  color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLuint loc = glGetAttribLocation(shader->getProg(), "position");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "position"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const GLvoid*)(0*sizeof(float)));

    loc = glGetAttribLocation(shader->getProg(), "color");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "color"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const GLvoid*)(3*sizeof(float)));

    loc = glGetAttribLocation(shader->getProg(), "normal");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "normal"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const GLvoid*)(6*sizeof(float)));

	loc = glGetAttribLocation(shader->getProg(), "uv");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "normal"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 11*sizeof(float), (const GLvoid*)(9*sizeof(float)));

    glBindVertexArray(0);

    return vertexArrayHandle;
}


/**
 * This function will create the sphere geometry.
 * It uses the same algorithm as in Three.js SphereGeometry class:
 * http://threejs.org/docs/#Reference/Extras.Geometries/SphereGeometry
 */
GLuint create_sphere(float radius, int width_segments, int height_segments, glm::vec3 color, int &chopper_body_triangle_count, shader_prog* shader) {
    GLuint vaoHandle;

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

	float phiStart = 0.0;
	float phiLength = glm::pi<float>() * 2.0;

	float thetaStart = 0.0;
	float thetaLength = glm::pi<float>();

	int x, y;
	float u, v;
	std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
	std::vector<glm::vec3> normals = std::vector<glm::vec3>();
	std::vector<glm::vec3> colors = std::vector<glm::vec3>();
	std::vector<glm::u16vec3> faces = std::vector<glm::u16vec3>();
	std::vector<unsigned short> indices = std::vector<unsigned short>();

	for(y = 0; y <= height_segments; y++) {
		for(x = 0; x <= width_segments; x++) {
			u = static_cast<float>(x) / width_segments;
			v = static_cast<float>(y) / height_segments;

			glm::vec3 vertex = glm::vec3(
                -radius * glm::cos(phiStart + u * phiLength) * glm::sin(thetaStart + v * thetaLength),
                radius * glm::cos(thetaStart + v * thetaLength),
                radius * glm::sin(phiStart + u * phiLength) * glm::sin(thetaStart + v * thetaLength)
            );

			vertices.push_back(vertex);
			normals.push_back(glm::normalize(vertex));
			colors.push_back(color);
		}
	}

	int vIndex1, vIndex2, vIndex3, vIndex4;

	for(y = 0; y < height_segments; y++) {
		for(x = 0; x < width_segments; x++) {

            vIndex1 = (width_segments+1) * y + x+1;
            vIndex2 = (width_segments+1) * y + x;
            vIndex3 = (width_segments+1) * (y+1) + x;
            vIndex4 = (width_segments+1) * (y+1) + x + 1;

			auto v1 = vertices.at(vIndex1);
			auto v2 = vertices.at(vIndex2);
			auto v3 = vertices.at(vIndex3);
			auto v4 = vertices.at(vIndex4);

			if (glm::abs(v1.y) == radius) {
                faces.emplace_back(vIndex1, vIndex3, vIndex4);
			} else if(glm::abs(v3.y) == radius) {
                faces.emplace_back(vIndex1, vIndex2, vIndex3);
			} else {
                faces.emplace_back(vIndex1, vIndex2, vIndex4);
                faces.emplace_back(vIndex2, vIndex3, vIndex4);
			}
			chopper_body_triangle_count += 3;
		}
		chopper_body_triangle_count += 3;
	}

    shader->use();
    shader->attributeVectorVec3("position", vertices);
    shader->attributeVectorVec3("color", colors);
    shader->attributeVectorVec3("normal", normals);

    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*faces.size()*3, &faces[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    return vaoHandle;
}

/**
 * This function creates a cube.
 * Here we use another approach to define the geometry: one VBO with a stride to hold different data.
 */
GLuint create_cube(glm::vec3 color, shader_prog* shader) {
    GLuint vertexArrayHandle;
    GLuint arrayBufferHandle;

    glGenVertexArrays(1, &vertexArrayHandle);
    glBindVertexArray(vertexArrayHandle);
    glGenBuffers(1, &arrayBufferHandle);

    GLfloat vertexData[] = {
        //  X     Y     Z      Red      Green    Blue       Normal
        // bottom
        -1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, -1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,   color.r, color.g, color.b,   0.0f, -1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, -1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,   color.r, color.g, color.b,   0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,   color.r, color.g, color.b,   0.0f, -1.0f, 0.0f,

        // top
        -1.0f, 1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,  1.0f,   color.r, color.g, color.b,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,  1.0f,   color.r, color.g, color.b,   0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,  1.0f,   color.r, color.g, color.b,   0.0f, 1.0f, 0.0f,

        // front
        -1.0f, -1.0f, 1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, 1.0f,

        // back
        -1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,   color.r, color.g, color.b,   0.0f, 0.0f, -1.0f,

        // left
        -1.0f, -1.0f,  1.0f,   color.r, color.g, color.b,   -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,   color.r, color.g, color.b,   -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,   color.r, color.g, color.b,   -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,   color.r, color.g, color.b,   -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,   color.r, color.g, color.b,   -1.0f, 0.0f, 0.0f,

        // right
         1.0f, -1.0f,  1.0f,   color.r, color.g, color.b,   1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,   color.r, color.g, color.b,   1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,   color.r, color.g, color.b,   1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,   color.r, color.g, color.b,   1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,   color.r, color.g, color.b,   1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,   color.r, color.g, color.b,   1.0f, 0.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLuint loc = glGetAttribLocation(shader->getProg(), "position");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "position"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (const GLvoid*)(0*sizeof(float)));


    loc = glGetAttribLocation(shader->getProg(), "color");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "color"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (const GLvoid*)(3*sizeof(float)));

    loc = glGetAttribLocation(shader->getProg(), "normal");
    if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "normal"));
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (const GLvoid*)(6*sizeof(float)));

    glBindVertexArray(0);
    return vertexArrayHandle;
}
