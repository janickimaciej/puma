#include "box.hpp"

#include "shaderPrograms.hpp"

#include <cmath>
#include <cstddef>

Box::Box(const glm::vec3& color) :
	m_color{color}
{
	std::vector<Mesh::Vertex> vertices(24);
	vertices[0].pos = {-0.5f, -0.5f, -0.5f};
	vertices[1].pos = {-0.5f, 0.5f, -0.5f};
	vertices[2].pos = {0.5f, -0.5f, -0.5f};
	vertices[3].pos = {0.5f, 0.5f, -0.5f};
	for (int i = 0; i < 4; ++i)
	{
		vertices[i].normalVector = {0, 0, -1};
	}

	std::vector<unsigned int> indices(36);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	static constexpr glm::mat3 rotationX90Deg
	{
		1, 0, 0,
		0, 0, 1,
		0, -1, 0
	};

	static constexpr glm::mat3 rotationY90Deg
	{
		0, 0, -1,
		0, 1, 0,
		1, 0, 0
	};

	for (std::size_t i = 0; i < 3; ++i)
	{
		for (std::size_t j = 0; j < 4; ++j)
		{
			vertices[4 * (i + 1) + j].pos = rotationX90Deg * vertices[4 * i + j].pos;
			vertices[4 * (i + 1) + j].normalVector =
				rotationX90Deg * vertices[4 * i + j].normalVector;
		}
	}

	for (std::size_t i = 0; i < 4; ++i)
	{
		vertices[16 + i].pos = rotationY90Deg * vertices[i].pos;
		vertices[16 + i].normalVector = rotationY90Deg * vertices[i].normalVector;
		vertices[20 + i].pos = rotationY90Deg * rotationY90Deg * vertices[16 + i].pos;
		vertices[20 + i].normalVector =
			rotationY90Deg * rotationY90Deg * vertices[16 + i].normalVector;
	}

	for (std::size_t i = 0; i < 5; ++i)
	{
		for (std::size_t j = 0; j < 6; ++j)
		{
			indices[6 * (i + 1) + j] = indices[6 * i + j] + 4;
		}
	}

	m_mesh = std::make_unique<Mesh>(vertices, indices);
}

void Box::render() const
{
	updateShaders();
	m_mesh->render();
}

void Box::updateShaders() const
{
	ShaderPrograms::mesh->use();
	ShaderPrograms::mesh->setUniform("modelMatrix", getMatrix());
	ShaderPrograms::mesh->setUniform("color", m_color);
}
