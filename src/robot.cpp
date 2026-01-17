#include "robot.hpp"

#include "utils.hpp"

#include <glm/gtc/constants.hpp>

Robot::Robot()
{
	glm::vec3 boxesColor{0.8f, 0.8f, 0.8f};
	for (int i = 0; i < 4; ++i)
	{
		m_boxes.push_back(std::make_unique<Box>(boxesColor));
	}

	updateBoxes();
}

void Robot::render() const
{
	for (const auto& box : m_boxes)
	{
		box->render();
	}
}

void Robot::setConfig(const Config& config)
{
	m_config = config;
	updateBoxes();
}

glm::vec3 Robot::configToPos(const Config& config)
{
	return rotationY(config.alpha1Rad) * translation({0, m_geometry.l1, 0}) *
		rotationZ(-config.alpha2Rad) * translation({config.q2, 0, 0}) *
		rotationZ(-config.alpha3Rad) * translation({0, -m_geometry.l3, 0}) *
		rotationY(config.alpha4Rad) * translation({m_geometry.l4, 0, 0}) *
		rotationX(config.alpha5Rad) * glm::vec4{0, 0, 0, 1};
}

glm::mat4 Robot::configToRotationMatrix(const Config& config)
{
	return rotationY(config.alpha1Rad) * rotationZ(-config.alpha2Rad) *
		rotationZ(-config.alpha3Rad) * rotationY(config.alpha4Rad) * rotationX(config.alpha5Rad);
}

Robot::Config Robot::posAndQuatToConfig(const glm::vec3& pos, const glm::vec4& quat,
	const Config& prevConfig)
{
	static constexpr float eps = 1e-9f;

	glm::vec3 p1 = {0, 0, 0};
	glm::vec3 p2 = {0, m_geometry.l1, 0};
	glm::vec3 p5 = pos;

	glm::mat4 rotationMatrix = quatToRotationMatrix(quat);
	glm::vec3 x5{rotationMatrix[0][0], rotationMatrix[0][1], rotationMatrix[0][2]};
	glm::vec3 y5{rotationMatrix[1][0], rotationMatrix[1][1], rotationMatrix[1][2]};

	glm::vec3 p4 = p5 - m_geometry.l4 * x5;

	glm::vec3 n = glm::cross(p2 - p1, p4 - p1);
	glm::vec3 p4p3 = glm::cross(n, x5);

	glm::vec3 prevY4 = rotationY(prevConfig.alpha1Rad) * rotationZ(-prevConfig.alpha2Rad) *
		rotationZ(-prevConfig.alpha3Rad) * rotationY(prevConfig.alpha4Rad) * glm::vec4{0, 1, 0, 0};
	glm::vec3 y4{};

	if (glm::length(p4p3) < eps)
	{
		glm::vec3 tmp = glm::cross(x5, prevY4);
		if (glm::length(tmp) < eps)
		{
			y4 = glm::cross(x5, glm::vec3{1, 0, 0});
			if (glm::length(y4) < eps)
			{
				y4 = glm::cross(x5, glm::vec3{0, 1, 0});
			}
		}
		else
		{
			y4 = glm::cross(tmp, x5);
		}
		y4 = glm::normalize(y4);
	}
	else
	{
		y4 = glm::normalize(p4p3);
		if (glm::dot(-y4, prevY4) > glm::dot(y4, prevY4))
		{
			y4 = -y4;
		}
	}

	glm::vec3 p3 = p4 + m_geometry.l3 * y4;

	Config config{};

	if (glm::length(glm::vec2{p4.x, p4.z}) < eps)
	{
		if (glm::length(glm::vec2{p3.x, p3.z}) < eps)
		{
			config.alpha1Rad = prevConfig.alpha1Rad;
		}
		else
		{
			config.alpha1Rad = std::atan2(-p3.z, p3.x);
		}
	}
	else
	{
		config.alpha1Rad = std::atan2(-p4.z, p4.x);
	}
	config.alpha1Rad = ::normalize(config.alpha1Rad);
	float diff = std::abs(config.alpha1Rad - prevConfig.alpha1Rad);
	if (diff > glm::pi<float>() / 2.0f && diff < glm::pi<float>() * 3.0f / 2.0f)
	{
		config.alpha1Rad += glm::pi<float>();
	}

	glm::vec3 p2p3Local = rotationY(-config.alpha1Rad) * glm::vec4((p3 - p2), 0);
	config.alpha2Rad = std::atan2(-p2p3Local.y, p2p3Local.x);

	config.q2 = glm::length(p3 - p2);

	glm::vec3 p3p4Local =
		rotationZ(config.alpha2Rad) * rotationY(-config.alpha1Rad) * glm::vec4((p4 - p3), 0);
	config.alpha3Rad = std::atan2(-p3p4Local.x, -p3p4Local.y);

	glm::vec3 p4p5Local = rotationZ(config.alpha3Rad) * rotationZ(config.alpha2Rad) *
		rotationY(-config.alpha1Rad) * glm::vec4((p5 - p4), 0);
	config.alpha4Rad = std::atan2(-p4p5Local.z, p4p5Local.x);

	glm::vec3 y5Local = rotationY(-config.alpha4Rad) * rotationZ(config.alpha3Rad) *
		rotationZ(config.alpha2Rad) * rotationY(-config.alpha1Rad) * glm::vec4(y5, 0);
	config.alpha5Rad = std::atan2(y5Local.z, y5Local.y);

	return normalize(config);
}

Robot::Geometry Robot::m_geometry{};

void Robot::updateBoxes()
{
	static constexpr float boxesSize = 0.05f;

	glm::mat4 rotationMatrix{1};
	glm::mat4 posMatrix{1};
	glm::mat4 currRotation{};
	glm::mat4 currTranslation{};

	currRotation = rotationY(m_config.alpha1Rad);
	currTranslation = translation({0, m_geometry.l1 / 2.0f, 0});
	rotationMatrix *= currRotation;
	posMatrix *= currRotation * currTranslation;
	m_boxes[0]->setScale({boxesSize, m_geometry.l1, boxesSize});
	m_boxes[0]->setPosition(posMatrix * glm::vec4{0, 0, 0, 1});
	m_boxes[0]->setRotationMatrix(rotationMatrix);
	posMatrix *= currTranslation;

	currRotation = rotationZ(-m_config.alpha2Rad);
	currTranslation = translation({m_config.q2 / 2.0f, 0, 0});
	rotationMatrix *= currRotation;
	posMatrix *= currRotation * currTranslation;
	m_boxes[1]->setScale({m_config.q2, boxesSize, boxesSize});
	m_boxes[1]->setPosition(posMatrix * glm::vec4{0, 0, 0, 1});
	m_boxes[1]->setRotationMatrix(rotationMatrix);
	posMatrix *= currTranslation;

	currRotation = rotationZ(-m_config.alpha3Rad);
	currTranslation = translation({0, -m_geometry.l3 / 2.0f, 0});
	rotationMatrix *= currRotation;
	posMatrix *= currRotation * currTranslation;
	m_boxes[2]->setScale({boxesSize, m_geometry.l3, boxesSize});
	m_boxes[2]->setPosition(posMatrix * glm::vec4{0, 0, 0, 1});
	m_boxes[2]->setRotationMatrix(rotationMatrix);
	posMatrix *= currTranslation;

	currRotation = rotationY(m_config.alpha4Rad);
	currTranslation = translation({m_geometry.l4 / 2.0f, 0, 0});
	rotationMatrix *= currRotation;
	posMatrix *= currRotation * currTranslation;
	m_boxes[3]->setScale({m_geometry.l4, boxesSize, boxesSize});
	m_boxes[3]->setPosition(posMatrix * glm::vec4{0, 0, 0, 1});
	m_boxes[3]->setRotationMatrix(rotationMatrix);
	posMatrix *= currTranslation;
}

glm::mat4 Robot::getEndMatrix(const Config& config)
{
	return rotationY(config.alpha1Rad) * translation({0, m_geometry.l1, 0}) *
		rotationZ(-config.alpha2Rad) * translation({config.q2, 0, 0}) *
		rotationZ(-config.alpha3Rad) * translation({0, -m_geometry.l3, 0}) *
		rotationY(config.alpha4Rad) * translation({m_geometry.l4, 0, 0}) *
		rotationX(config.alpha5Rad);
}

Robot::Config Robot::normalize(const Config& config)
{
	Config normalizedConfig{};
	normalizedConfig.alpha1Rad = ::normalize(config.alpha1Rad);
	normalizedConfig.alpha2Rad = ::normalize(config.alpha2Rad);
	normalizedConfig.q2 = config.q2;
	normalizedConfig.alpha3Rad = ::normalize(config.alpha3Rad);
	normalizedConfig.alpha4Rad = ::normalize(config.alpha4Rad);
	normalizedConfig.alpha5Rad = ::normalize(config.alpha5Rad);
	return normalizedConfig;
}
