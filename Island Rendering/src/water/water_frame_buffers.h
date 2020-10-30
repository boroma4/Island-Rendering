#pragma once
#include <GLEW/glew.h>
#include "../Settings.h"

class water_frame_buffers
{
public:
	// TODO: change to private with getters
	GLuint reflection_frame_buffer;
	GLuint reflection_texture;
	GLuint reflection_depth_buffer;
	GLuint refraction_frame_buffer;
	GLuint refraction_texture;
	GLuint refraction_depth_texture;

public:
	water_frame_buffers();
	~water_frame_buffers();
	void unbind_current_fbo();
	void bind_reflection_fbo();
	void bind_refraction_fbo();
	

private:
	const int REFLECTION_WIDTH = 320;
	const int REFLECTION_HEIGHT = 180;
	
	const int REFRACTION_WIDTH = 1280;
	const int REFRACTION_HEIGHT = 720;

private:
	void init_refraction_fbo();
	void init_reflection_fbo();
	void bind_fbo(const GLuint buffer, const int w, const int h);
	GLuint create_fbo();
	GLuint create_texture_attachment(const int w, const int h);
	GLuint create_depth_texture_attachment(const int w, const int h);
	GLuint create_depth_buffer_attachment(const int w, const int h);
};
