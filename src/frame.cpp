#include "frame.hpp"

#include "shaderPrograms.hpp"

Frame::Frame(bool intermediate) :
	m_intermediate{intermediate}
{
	if (m_mainFrameMesh == nullptr)
	{
		m_mainFrameMesh = std::make_unique<FrameMesh>(false);
		m_intermediateFrameMesh = std::make_unique<FrameMesh>(true);
	}
}

void Frame::render() const
{
	updateShaders();
	if (m_intermediate)
	{
		m_intermediateFrameMesh->render();
	}
	else
	{
		m_mainFrameMesh->render();
	}
}

glm::vec3 Frame::getPos() const
{
	return m_pos;
}

void Frame::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	updateModelMatrix();
}

void Frame::setRotationMatrix(const glm::mat4& matrix)
{
	m_rotationMatrix = matrix;
	updateModelMatrix();
}

std::unique_ptr<FrameMesh> Frame::m_mainFrameMesh = nullptr;
std::unique_ptr<FrameMesh> Frame::m_intermediateFrameMesh = nullptr;

void Frame::updateShaders() const
{
	ShaderPrograms::frame->use();
	ShaderPrograms::frame->setUniform("modelMatrix", m_modelMatrix);
}

void Frame::updateModelMatrix()
{
	glm::mat4 posMatrix
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_pos.x, m_pos.y, m_pos.z, 1
	};

	m_modelMatrix = posMatrix * m_rotationMatrix;
}
