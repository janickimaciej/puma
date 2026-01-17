#pragma once

#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 pos{};
		glm::vec3 normalVector{};
	};

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	void render() const;

private:
	std::size_t m_indexCount{};
	unsigned int m_VBO{};
	unsigned int m_EBO{};
	unsigned int m_VAO{};

	void createVBO(const std::vector<Vertex>& vertices);
	void createEBO(const std::vector<unsigned int>& indices);
	void createVAO();
};
