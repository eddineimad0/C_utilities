#include "VAO.h"

VAO::VAO()
{
	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &ID);
}

void VAO::linkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	vbo.bind();
	glVertexAttribPointer(layout, numComponents, type,GL_FALSE,stride, offset);
	// Enable the VAO so that OpenGL knows to use it
	glEnableVertexAttribArray(layout);
	vbo.unbind();

}

void VAO::bind()
{
	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(ID);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &ID);
}
