#pragma once

/**
 * MTAT.03.015 Computer Graphics.
 * Shader configuration utility routines.
 */


#include <vector>
#include <GLEW/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

/**
 * Modified version of code from:
 *  http://stackoverflow.com/questions/2795044/easy-framework-for-opengl-shaders-in-c-c
 */
class shader_prog {
	
private:
    GLuint vertex_shader, fragment_shader, prog;
    std::string v_source, f_source;
	std::unordered_map<std::string, GLuint> texture_map;
	static int textureCounter;
public:
    shader_prog(const char* vertex_shader_filename, const char* fragment_shader_filename);
    void use();
    void activate();
    void free();
    operator GLuint();

    // Shorthands for glUniform specification
    void uniform1i(const char* name, int i);
    void uniform1f(const char* name, float f);
    void uniform3f(const char* name, float x, float y, float z);
    void uniformMatrix4fv(const char* name, const float* matrix);
    void uniformMatrix4fv(const char* name, glm::mat4 matrix);
    void uniformVec2(const char* name, glm::vec2 v);
    void uniformVec3(const char* name, glm::vec3 v);
	void uniformVec4(const char* name, glm::vec4 v);
    void uniformTex2D(const char* name, GLuint texturePointer);
    void attribute3fv(const char* name, GLfloat* vecArray, int numberOfVertices);
    GLuint attributeVectorVec3(const char* name, std::vector<glm::vec3> vectorVec3);
    GLuint getProg() {
        return prog;
    }
};