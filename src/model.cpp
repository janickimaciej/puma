#include "model.hpp"

void Model::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	updateMatrix();
}

void Model::setRotationMatrix(const glm::mat4& matrix)
{
	m_rotationMatrix = matrix;
	updateMatrix();
}

void Model::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	updateMatrix();
}

glm::mat4 Model::getMatrix() const
{
	return m_modelMatrix;
}

void Model::updateMatrix()
{
	glm::mat4 scaleMatrix
		{
			m_scale.x, 0, 0, 0,
			0, m_scale.y, 0, 0,
			0, 0, m_scale.z, 0,
			0, 0, 0, 1
		};

	glm::mat4 posMatrix
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_pos.x, m_pos.y, m_pos.z, 1
		};

	m_modelMatrix = posMatrix * m_rotationMatrix * scaleMatrix;
}
