#include "EBO.h"

EBO::EBO(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &ID);
	//Bind the VBO specifying it's an Array Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// load the vertices data int the vbo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()* sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

}

EBO::~EBO()
{
	glDeleteBuffers(1, &ID);
}

void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
