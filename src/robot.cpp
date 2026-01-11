#include "robot.hpp"

void Robot::render() const
{
	// TODO
}

Robot::Config Robot::getConfig() const
{
	return m_config;
}

void Robot::setConfig(const Config& config)
{
	m_config = config;
}

void Robot::setPosAndOrientation(const glm::vec3& pos, const glm::vec4& quat)
{
	// TODO
}

glm::vec3 Robot::configToPos(const Config& config)
{
	return {}; // TODO
}

glm::mat4 Robot::configToRotationMatrix(const Config& config)
{
	return {}; // TODO
}
