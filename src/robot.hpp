#pragma once

#include "box.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Robot
{
public:
	struct Config
	{
		float alpha1Rad{};
		float alpha2Rad{};
		float q2{};
		float alpha3Rad{};
		float alpha4Rad{};
		float alpha5Rad{};
	};

	struct Geometry
	{
		float l1 = 1.0f;
		float l3 = 0.5f;
		float l4 = 0.5f;
	};

	Robot();

	void render() const;
	void setConfig(const Config& config);

	static glm::vec3 configToPos(const Config& config);
	static glm::mat4 configToRotationMatrix(const Config& config);
	static Config posAndQuatToConfig(const glm::vec3& pos, const glm::vec4& quat,
		const Config& prevConfig);

private:
	static Geometry m_geometry;
	Config m_config{};
	std::vector<std::unique_ptr<Box>> m_boxes{};

	void updateBoxes();

	static glm::mat4 getEndMatrix(const Config& config);
	static Config normalize(const Config& config);
};
