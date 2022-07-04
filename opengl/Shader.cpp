#include "shader.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile,const char* fragmentFile) {
	std::string vertexContent = get_file_contents(vertexFile);
	std::string fragmentContent = get_file_contents(fragmentFile);
	const char* vertexSource = vertexContent.c_str();
	const char* fragmentSource = fragmentContent.c_str();

	// Create a Vertex shader and get its index or refrence
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to Vertex shader refrence
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create a fragment shader and get its refrence
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment Shader src code to shader refrence
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	// Compile the Fragment shader into machine code
	glCompileShader(fragmentShader);

	// Create a Shader program and get its refrence
	ID = glCreateProgram();

	// Attach the Vertex and fragment Shaders to Program instance
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	// Link Compiled attached code to create the main shader program
	glLinkProgram(this->ID);
	// Delete the useless Vertex and Frament shader Object (~=.obj)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate() {
	glUseProgram(this->ID);
}

void Shader::compileError(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader Compilation Error : " << type << std::endl;
			std::cout << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader Linking Error : " << type << std::endl;
			std::cout << infoLog << std::endl;
		}
	}
}

Shader::~Shader() {
	glDeleteProgram(this->ID);
}