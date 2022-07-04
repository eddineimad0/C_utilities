#pragma once
#ifndef SHADER_CLASS
#define SHADER_CLASS

#include<glad/glad.h>
#include<sstream>
#include<fstream>
#include<string>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const Shader&) = delete;
	~Shader();
	void activate();
	void compileError(unsigned int shader, const char* type);
};


#endif