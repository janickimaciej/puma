#pragma once

#include "frame.hpp"
#include "robot.hpp"

#include <glm/glm.hpp>

#include <chrono>
#include <vector>

class Interpolation
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock,
		std::chrono::duration<float, std::nano>>;

public:
	Interpolation(Frame& configFrame, std::vector<Frame>& configFrames, Robot& configRobot,
		Frame& quatFrame, std::vector<Frame>& quatFrames, Robot& quatRobot);
	void start();
	void stop();
	void reset();
	void update();
	void updateFramesAndRobots();
	float getTime() const;
	float getEndTime() const;
	void setEndTime(float time);

	glm::vec3 getStartPos() const;
	void setStartPos(const glm::vec3& pos);
	glm::vec3 getStartEulerAngles() const;
	void setStartEulerAngles(const glm::vec3& eulerAngles);
	glm::vec4 getStartQuat() const;
	void setStartQuat(const glm::vec4& quat);
	void normalizeStartQuat();

	glm::vec3 getEndPos() const;
	void setEndPos(const glm::vec3& pos);
	glm::vec3 getEndEulerAngles() const;
	void setEndEulerAngles(const glm::vec3& eulerAngles);
	glm::vec4 getEndQuat() const;
	void setEndQuat(const glm::vec4& quat);
	void normalizeEndQuat();

private:
	Frame& m_configFrame;
	std::vector<Frame>& m_configFrames;
	Robot& m_configRobot;
	Frame& m_quatFrame;
	std::vector<Frame>& m_quatFrames;
	Robot& m_quatRobot;
	Robot::Config m_prevConfig{};

	TimePoint m_startTime{};
	float m_currentTime = 0;
	float m_endTime = 5;
	bool m_running = false;

	glm::vec3 m_startPos{-1, 0, 0};
	glm::vec3 m_startEulerAngles{0, 0, 0};
	glm::vec4 m_startQuat{0, 0, 0, 1};
	Robot::Config m_startConfig{};

	glm::vec3 m_endPos{1, 0, 0};
	glm::vec3 m_endEulerAngles{0, 0, 0};
	glm::vec4 m_endQuat{0, 0, 0, 1};
	Robot::Config m_endConfig{};

	void updateStartConfig();
	void updateEndConfig();
	glm::vec3 interpolatePos(float time) const;
	Robot::Config interpolateConfig(float time) const;
	float interpolateAngle(float start, float end, float time) const;
	glm::vec4 interpolateQuat(float time) const;
	static TimePoint now();
};
