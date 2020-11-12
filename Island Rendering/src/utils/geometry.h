#pragma once

#include <GLEW/glew.h>      // OpenGL Extension Wrangler -
#include <glm/glm.hpp>      // OpenGL math library
#include "shader_util.h"
#include <stdexcept>


GLuint create_quad(glm::vec3 color, shader_prog* shader);
GLuint create_perlin_quad(glm::vec3 color, shader_prog* shader);
GLuint create_textured_quad(glm::vec3 color, shader_prog* shader);
GLuint create_sphere(float radius, int width_segments, int height_segments, glm::vec3 color, int &chopper_body_triangle_count, shader_prog* shader);
GLuint create_cube(glm::vec3 color, shader_prog* shader);

