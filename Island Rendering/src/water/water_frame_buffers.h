﻿#pragma once
#include <GLEW/glew.h>
#include "../Settings.h"

class water_frame_buffers
{
public:
	// TODO: change to private with getters
	GLuint reflection_frame_buffer;
	GLuint reflection_texture;
	GLuint refraction_frame_buffer;
	GLuint refraction_texture;
	GLuint depth_texture;

public:
	water_frame_buffers();
	~water_frame_buffers();
	void on_screen_resize(int w, int h);
	void unbind_current_fbo();
	void bind_reflection_fbo();
	void bind_refraction_fbo();
	
private:
	const int REFLECTION_WIDTH = 640;
	const int REFLECTION_HEIGHT = 480;
	const int REFRACTION_WIDTH = 640;
	const int REFRACTION_HEIGHT = 480;

	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

private:
	void init_refraction_fbo();
	void init_reflection_fbo();
	void bind_fbo(const GLuint buffer, const int w, const int h);
	GLuint create_fbo();
	GLuint create_texture_attachment(const int w, const int h);
	GLuint create_depth_texture_attachment(const int w, const int h);
};
