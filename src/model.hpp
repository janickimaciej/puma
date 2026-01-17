#pragma once

#include "shaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Model
{
public:
	virtual void render() const = 0;
	void setPosition(const glm::vec3& position);
	void setRotationMatrix(const glm::mat4& matrix);
	void setScale(const glm::vec3& scale);

protected:
	glm::mat4 getMatrix() const;

private:
	glm::mat4 m_modelMatrix{1};

	glm::vec3 m_position{};
	glm::mat4 m_rotationMatrix{1};
	glm::vec3 m_scale{1, 1, 1};

	void updateMatrix();
};
