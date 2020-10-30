#include "water_frame_buffers.h"

water_frame_buffers::water_frame_buffers()
{
	init_reflection_fbo();
	init_refraction_fbo();
}

water_frame_buffers::~water_frame_buffers()
{
	glDeleteTextures(1, &reflection_texture);
	glDeleteTextures(1, &refraction_texture);
	glDeleteTextures(1, &refraction_depth_texture);
	glDeleteFramebuffers(1, &reflection_frame_buffer);
	glDeleteFramebuffers(1, &refraction_frame_buffer);
	glDeleteRenderbuffers(1, &reflection_depth_buffer);
}

void water_frame_buffers::unbind_current_fbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void water_frame_buffers::bind_reflection_fbo()
{
	bind_fbo(reflection_frame_buffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void water_frame_buffers::bind_refraction_fbo()
{
	bind_fbo(refraction_frame_buffer, REFRACTION_WIDTH, REFLECTION_HEIGHT);
}

void water_frame_buffers::init_refraction_fbo()
{
		refraction_frame_buffer = create_fbo();
		refraction_texture = create_texture_attachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
		refraction_depth_texture = create_depth_texture_attachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
		unbind_current_fbo();
}

void water_frame_buffers::init_reflection_fbo()
{
	    reflection_frame_buffer = create_fbo();
		reflection_texture = create_texture_attachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
		reflection_depth_buffer = create_depth_buffer_attachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
		unbind_current_fbo();
}

void water_frame_buffers::bind_fbo(const GLuint buffer, const int w, const int h)
{
	glBindTexture(GL_TEXTURE_2D, 0); // just in case
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glViewport(0, 0, w, h);
}

GLuint water_frame_buffers::create_fbo()
{
	GLuint buf;
	glGenFramebuffers(1, &buf);
    glBindFramebuffer(GL_FRAMEBUFFER, buf);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	
	return buf;
}

GLuint water_frame_buffers::create_texture_attachment(const int w, const int h)
{
	GLuint tex;
    glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);

	return tex;
}

GLuint water_frame_buffers::create_depth_texture_attachment(const int w, const int h)
{
	GLuint tex;
    glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex, 0);

	return tex;
}

GLuint water_frame_buffers::create_depth_buffer_attachment(const int w, const int h)
{
	GLuint buf;
    glGenTextures(1, &buf);
	glBindRenderbuffer(GL_RENDERBUFFER, buf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buf);

	return buf;
}
