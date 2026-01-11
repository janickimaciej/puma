#pragma once

#include <glm/glm.hpp>

class Robot
{
public:
	struct Config
	{
		float alpha1;
		float alpha2;
		float q2;
		float alpha3;
		float alpha4;
		float alpha5;
	};

	struct Geometry
	{
		float l1 = 1.0f;
		float l3 = 0.5f;
		float l4 = 0.5f;
	};

	void render() const;
	Config getConfig() const;
	void setConfig(const Config& config);
	void setPosAndOrientation(const glm::vec3& pos, const glm::vec4& quat);

	static glm::vec3 configToPos(const Config& config);
	static glm::mat4 configToRotationMatrix(const Config& config);

private:
	Geometry m_geometry{};
	Config m_config{};
};
