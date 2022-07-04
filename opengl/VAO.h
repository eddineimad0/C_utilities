#pragma once
#ifndef VAO_CLASS
#define VAO_CLASS

#include <glad/glad.h>
#include "VBO.h"
class VAO
{
public:
	GLuint ID;
	VAO();
	VAO(const VAO&) = delete;

	void linkAttrib(VBO& vbo, GLuint layout,GLuint numComponents,GLenum type,GLsizeiptr stride,void* offset);
	void bind();
	void unbind();
	~VAO();

};

#endif 