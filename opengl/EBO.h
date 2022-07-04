#pragma once
#ifndef EBO_CLASS
#define EBO_CLASS


#include<glad/glad.h>
#include<vector>

class EBO
{
public:
	GLuint ID;
	EBO(std::vector<GLuint>& indices);
	EBO(const EBO&) = delete;
	~EBO();
	void bind();
	void unbind();
};

#endif 