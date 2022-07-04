#include "VBO.h"

VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	//Bind the VBO specifying it's an Array Buffer
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// load the vertices data int the vbo
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

}

VBO::~VBO()
{
	glDeleteBuffers(1, &ID);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
