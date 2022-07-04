#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->texutres = textures;

	vao.bind();
	VBO vbo1(vertices);
	EBO ebo1(indices);

	vao.linkAttrib(vbo1, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
	vao.linkAttrib(vbo1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	vao.linkAttrib(vbo1, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(GLfloat)));
	vao.linkAttrib(vbo1, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(GLfloat)));
	vao.unbind();
	vbo1.unbind();
	ebo1.unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.activate();
	vao.bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < texutres.size(); i++)
	{
		std::string num;
		std::string type = texutres[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		texutres[i].texUnit(shader, (type + num).c_str(),i);
		texutres[i].Bind();
	}
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
