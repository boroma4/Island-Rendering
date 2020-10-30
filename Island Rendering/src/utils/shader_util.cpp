﻿/**
 * MTAT.03.015 Computer Graphics.
 * Shader configuration utility routines.
 */
#define _CRT_SECURE_NO_WARNINGS

#include "shader_util.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>
using std::strcpy;

// -------- Utility functions --------------
/**
 * Reads file contents into a string.
 */
std::string get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) return std::string((std::istreambuf_iterator<char>(in)),
                                std::istreambuf_iterator<char>());
    else throw(std::runtime_error(std::string("Failed to read file ") + filename));
}

bool invalidChar (char c) 
{  
    return !(c>=0 && c <128);   
} 
void stripUnicode(std::string & str) 
{ 
    str.erase(remove_if(str.begin(),str.end(), invalidChar), str.end());  
}

/**
 * Allocates and compiles given shader in OpenGL
 */
GLuint compile(GLuint type, std::string source) {
    GLuint shader = glCreateShader(type);
    // Split the code into separate lines (then the compilation error messages are more informative)
    std::vector<GLchar *> lines;
    std::string line;
    std::istringstream ss(source);
    while(getline(ss, line)) {
    	stripUnicode(line);
        line += '\n';
        GLchar* c_line = (GLchar*)malloc(line.size()+1);
        strcpy(c_line, line.c_str());
        lines.push_back(c_line);
    }

    // Compile the source
    glShaderSource(shader, lines.size(), (const GLchar**)&lines[0], NULL);
    glCompileShader(shader);
    for (auto& line : lines) free(line);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        std::cout << "Shader compilation error: " << log << std::endl;
        throw std::logic_error(log);
    }
    return shader;
}

/**
 * Default shaders.
 */
const GLchar *default_vertex_shader =
    "#version 120\n"
    "varying vec4 vertex_color\n;"
    "void main(void) {\n"
    "    gl_Position = ftransform();\n"
    "    vertex_color = gl_Color;\n"
    "}";

const GLchar *default_fragment_shader = // Only supports coloring. Textures require custom stuff.
    "#version 120\n"
    "varying vec4 vertex_color;\n"
    "void main() {\n"
    "    gl_FragColor = vertex_color;\n"
    "}";

shader_prog::shader_prog(const char* vertex_shader_filename, const char* fragment_shader_filename) {
    v_source = vertex_shader_filename == nullptr ? std::string(static_cast<const char*>(default_vertex_shader)) : get_file_contents(vertex_shader_filename);
    f_source = fragment_shader_filename == nullptr ? std::string(static_cast<const char*>(default_fragment_shader)) : get_file_contents(fragment_shader_filename);
}

void shader_prog::use() {
    vertex_shader = compile(GL_VERTEX_SHADER, v_source);
    fragment_shader = compile(GL_FRAGMENT_SHADER, f_source);
    prog = glCreateProgram();
    glAttachShader(prog, vertex_shader);
    glAttachShader(prog, fragment_shader);
    glLinkProgram(prog);
    glUseProgram(prog);
}

void shader_prog::activate() {
    glUseProgram(prog);
}

void shader_prog::free() {
    glDeleteProgram(prog);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glUseProgram(0);
}

shader_prog::operator GLuint() {
    return prog;
}

void shader_prog::uniform1i(const char* name, int i) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform1i(loc, i);
}
void shader_prog::uniform1f(const char* name, float f) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform1f(loc, f);
}
void shader_prog::uniform3f(const char* name, float x, float y, float z) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform3f(loc, x, y, z);
}
void shader_prog::uniformMatrix4fv(const char* name, const float* matrix) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
}
void shader_prog::uniformMatrix4fv(const char* name, glm::mat4 matrix) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void shader_prog::uniformVec2(const char* name, glm::vec2 v) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform2fv(loc, 1, glm::value_ptr(v));

}
void shader_prog::uniformVec3(const char* name, glm::vec3 v) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform3fv(loc, 1, glm::value_ptr(v));
}
void shader_prog::uniformVec4(const char* name, glm::vec4 v)
{
	GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform4fv(loc, 1, glm::value_ptr(v));
}
void shader_prog::uniformTex2D(const char* name, GLuint texturePointer) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glUniform1i(loc, textureCounter);
    glActiveTexture(GL_TEXTURE0 + textureCounter);
    glBindTexture(GL_TEXTURE_2D, texturePointer);
    textureCounter++;
}

void shader_prog::attribute3fv(const char* name, GLfloat* vecArray, int numberOfVertices) {
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numberOfVertices, vecArray, GL_STATIC_DRAW);

    GLuint loc = glGetAttribLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glEnableVertexAttribArray(loc);

    //printf("Enabled location: %d\n", loc);

    glVertexAttribPointer(
        loc, // attribute
        3,                 // number of elements per vertex, here (r,g,b)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        0                  // offset of first element
    );
}
GLuint shader_prog::attributeVectorVec3(const char* name, std::vector<glm::vec3> vectorVec3) {
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vectorVec3.size()*3, &vectorVec3[0], GL_STATIC_DRAW);

    GLuint loc = glGetAttribLocation(prog, name);
    if (loc < 0) printf("WARNING: Location not found in shader program for variable %s.\n", name);
    glEnableVertexAttribArray(loc);

    std::cout << name << ": " << loc << std::endl;

    glVertexAttribPointer(
        loc, // attribute
        3,                 // number of elements per vertex, here (r,g,b)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        0                  // offset of first element
    );

    return vboHandle;
}
