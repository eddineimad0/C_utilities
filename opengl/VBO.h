#pragma once
#ifndef VBO_CLASS
#define VBO_CLASS

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO
{
public:
	GLuint ID;
	VBO(std::vector<Vertex>& vertices);
	VBO(const VBO&) = delete;
	~VBO();
	void bind();
	void unbind();
};

#endif 