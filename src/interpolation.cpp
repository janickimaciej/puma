#include "interpolation.hpp"

#include "utils.hpp"

#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>

Interpolation::Interpolation(Frame& configFrame, std::vector<Frame>& configFrames,
	Robot& configRobot, Frame& quatFrame, std::vector<Frame>& quatFrames, Robot& quatRobot) :
	m_configFrame{configFrame},
	m_configFrames{configFrames},
	m_configRobot{configRobot},
	m_quatFrame{quatFrame},
	m_quatFrames{quatFrames},
	m_quatRobot{quatRobot}
{
	updateStartConfig();
	updateEndConfig();
	reset();
}

void Interpolation::start()
{
	if (m_running)
	{
		return;
	}

	m_startTime = now() - std::chrono::duration<float>(m_currentTime);
	m_running = true;
}

void Interpolation::stop()
{
	m_running = false;
}

void Interpolation::reset()
{
	stop();
	m_currentTime = 0;
	m_prevConfig = m_startConfig;
	updateFramesAndRobots();
}

void Interpolation::update()
{
	if (!m_running)
	{
		return;
	}

	m_currentTime =
		std::chrono::duration_cast<std::chrono::duration<float>>(now() - m_startTime).count();
	if (m_currentTime >= m_endTime)
	{
		m_currentTime = m_endTime;
		m_running = false;
	}
	updateFramesAndRobots();
}

void Interpolation::updateFramesAndRobots()
{
	m_configFrame.setPos(Robot::configToPos(interpolateConfig(m_currentTime)));
	m_configFrame.setRotationMatrix(
		Robot::configToRotationMatrix(interpolateConfig(m_currentTime)));

	m_quatFrame.setPos(interpolatePos(m_currentTime));
	m_quatFrame.setRotationMatrix(quatToRotationMatrix(interpolateQuat(m_currentTime)));

	std::size_t intermediateFrameCount = m_configFrames.size();
	float dTime = m_endTime / (intermediateFrameCount - 1);
	for (int i = 0; i < intermediateFrameCount; ++i)
	{
		float time = i * dTime;

		m_configFrames[i].setPos(Robot::configToPos(interpolateConfig(time)));
		m_configFrames[i].setRotationMatrix(Robot::configToRotationMatrix(interpolateConfig(time)));

		m_quatFrames[i].setPos(interpolatePos(time));
		m_quatFrames[i].setRotationMatrix(quatToRotationMatrix(interpolateQuat(time)));
	}

	m_configRobot.setConfig(interpolateConfig(m_currentTime));

	m_prevConfig = Robot::posAndQuatToConfig(interpolatePos(m_currentTime),
		interpolateQuat(m_currentTime), m_prevConfig);
	m_quatRobot.setConfig(m_prevConfig);
}

float Interpolation::getTime() const
{
	return m_currentTime;
}

float Interpolation::getEndTime() const
{
	return m_endTime;
}

void Interpolation::setEndTime(float time)
{
	m_endTime = time;
	reset();
}

glm::vec3 Interpolation::getStartPos() const
{
	return m_startPos;
}

void Interpolation::setStartPos(const glm::vec3& pos)
{
	m_startPos = pos;
	updateStartConfig();
	reset();
}

glm::vec3 Interpolation::getStartEulerAngles() const
{
	return m_startEulerAngles;
}

void Interpolation::setStartEulerAngles(const glm::vec3& eulerAngles)
{
	m_startEulerAngles = eulerAngles;
	m_startQuat = eulerAnglesToQuat(eulerAngles);
	updateStartConfig();
	reset();
}

glm::vec4 Interpolation::getStartQuat() const
{
	return m_startQuat;
}

void Interpolation::setStartQuat(const glm::vec4& quat)
{
	m_startQuat = quat;
	m_startEulerAngles = quatToEulerAngles(glm::normalize(quat));
	updateStartConfig();
	reset();
}

void Interpolation::normalizeStartQuat()
{
	m_startQuat = glm::normalize(m_startQuat);
}

glm::vec3 Interpolation::getEndPos() const
{
	return m_endPos;
}

void Interpolation::setEndPos(const glm::vec3& pos)
{
	m_endPos = pos;
	updateEndConfig();
	reset();
}

glm::vec3 Interpolation::getEndEulerAngles() const
{
	return m_endEulerAngles;
}

void Interpolation::setEndEulerAngles(const glm::vec3& eulerAngles)
{
	m_endEulerAngles = eulerAngles;
	m_endQuat = eulerAnglesToQuat(eulerAngles);
	updateEndConfig();
	reset();
}

glm::vec4 Interpolation::getEndQuat() const
{
	return m_endQuat;
}

void Interpolation::setEndQuat(const glm::vec4& quat)
{
	m_endQuat = quat;
	m_endEulerAngles = quatToEulerAngles(glm::normalize(quat));
	updateEndConfig();
	reset();
}

void Interpolation::normalizeEndQuat()
{
	m_endQuat = glm::normalize(m_endQuat);
}

void Interpolation::updateStartConfig()
{
	m_startConfig = Robot::posAndQuatToConfig(m_startPos, glm::normalize(m_startQuat), m_endConfig);
}

void Interpolation::updateEndConfig()
{
	m_endConfig = Robot::posAndQuatToConfig(m_endPos, glm::normalize(m_endQuat), m_startConfig);
}

glm::vec3 Interpolation::interpolatePos(float time) const
{
	return m_startPos + (m_endPos - m_startPos) * time / m_endTime;
}

Robot::Config Interpolation::interpolateConfig(float time) const
{
	Robot::Config config{};
	config.alpha1Rad = interpolateAngle(m_startConfig.alpha1Rad, m_endConfig.alpha1Rad, time);
	config.alpha2Rad = interpolateAngle(m_startConfig.alpha2Rad, m_endConfig.alpha2Rad, time);
	config.q2 = m_startConfig.q2 + (m_endConfig.q2 - m_startConfig.q2) * time / m_endTime;
	config.alpha3Rad = interpolateAngle(m_startConfig.alpha3Rad, m_endConfig.alpha3Rad, time);
	config.alpha4Rad = interpolateAngle(m_startConfig.alpha4Rad, m_endConfig.alpha4Rad, time);
	config.alpha5Rad = interpolateAngle(m_startConfig.alpha5Rad, m_endConfig.alpha5Rad, time);
	return config;
}

float Interpolation::interpolateAngle(float start, float end, float time) const
{
	if (end - start > glm::pi<float>()) end -= 2 * glm::pi<float>();
	if (start - end > glm::pi<float>()) start -= 2 * glm::pi<float>();
	return start + (end - start) * time / m_endTime;
}

glm::vec4 Interpolation::interpolateQuat(float time) const
{
	glm::vec4 start = glm::normalize(m_startQuat);
	glm::vec4 end = glm::normalize(m_endQuat);

	glm::vec4 startInv{-glm::vec3{start}, start.w};

	glm::vec4 product = quatProduct(startInv, end);
	glm::vec3 productV = product;
	float angle = 2 * std::atan2(glm::length(productV), product.w) * time / m_endTime;
	glm::vec3 axis = productV == glm::vec3{0, 0, 0} ? glm::vec3{0, 0, 0} : glm::normalize(productV);
	return quatProduct(start, glm::vec4{std::sin(angle / 2.0f) * axis, std::cos(angle / 2.0f)});
}

Interpolation::TimePoint Interpolation::now()
{
	return std::chrono::high_resolution_clock::now();
}
