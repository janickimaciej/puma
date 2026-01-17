#include "utils.hpp"

#include <glm/gtc/constants.hpp>

#include <cmath>

glm::vec4 eulerAnglesToQuat(const glm::vec3& eulerAngles)
{
	glm::vec4 quat{};

	float cx = std::cos(eulerAngles.x * 0.5f);
	float sx = std::sin(eulerAngles.x * 0.5f);
	float cy = std::cos(eulerAngles.y * 0.5f);
	float sy = std::sin(eulerAngles.y * 0.5f);
	float cz = std::cos(eulerAngles.z * 0.5f);
	float sz = std::sin(eulerAngles.z * 0.5f);

	quat.x = sx * cy * cz - cx * sy * sz;
	quat.y = cx * sy * cz + sx * cy * sz;
	quat.z = cx * cy * sz - sx * sy * cz;
	quat.w = cx * cy * cz + sx * sy * sz;

	return quat;
}

glm::vec3 quatToEulerAngles(const glm::vec4& quat)
{
	glm::vec3 eulerAngles{};

	eulerAngles.x = std::atan2(2 * (quat.w * quat.x + quat.y * quat.z),
		1 - 2 * (quat.x * quat.x + quat.y * quat.y));
	eulerAngles.y = std::asin(2 * (quat.w * quat.y - quat.x * quat.z));
	eulerAngles.z = std::atan2(2 * (quat.w * quat.z + quat.x * quat.y),
		1 - 2 * (quat.y * quat.y + quat.z * quat.z));

	return eulerAngles;
}

glm::mat4 quatToRotationMatrix(const glm::vec4& quat)
{
	float xx = quat.x * quat.x;
	float yy = quat.y * quat.y;
	float zz = quat.z * quat.z;
	float ww = quat.w * quat.w;
	float xy = quat.x * quat.y;
	float xz = quat.x * quat.z;
	float xw = quat.x * quat.w;
	float yz = quat.y * quat.z;
	float yw = quat.y * quat.w;
	float zw = quat.z * quat.w;

	return
		{
			ww + xx - yy - zz, 2 * (xy + zw), 2 * (xz - yw), 0,
			2 * (xy - zw), ww - xx + yy - zz, 2 * (yz + xw), 0,
			2 * (xz + yw), 2 * (yz - xw), ww - xx - yy + zz, 0,
			0, 0, 0, 1
		};
}

glm::vec4 quatProduct(const glm::vec4& q1, const glm::vec4& q2)
{
	glm::vec3 q1V = q1;
	glm::vec3 q2V = q2;
	glm::vec3 crossProduct = glm::cross(q1V, q2V);
	float dotProduct = glm::dot(q1V, q2V);
	glm::vec3 qV = crossProduct + q1.w * q2V + q2.w * q1V;
	return glm::vec4{qV, q1.w * q2.w - dotProduct};
}

glm::mat4 rotationX(float angleRad)
{
	float s = std::sin(angleRad);
	float c = std::cos(angleRad);
	return
		{
			1, 0, 0, 0,
			0, c, s, 0,
			0, -s, c, 0,
			0, 0, 0, 1
		};
}

glm::mat4 rotationY(float angleRad)
{
	float s = std::sin(angleRad);
	float c = std::cos(angleRad);
	return
		{
			c, 0, -s, 0,
			0, 1, 0, 0,
			s, 0, c, 0,
			0, 0, 0, 1
		};
}

glm::mat4 rotationZ(float angleRad)
{
	float s = std::sin(angleRad);
	float c = std::cos(angleRad);
	return
		{
			c, s, 0, 0,
			-s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
}

glm::mat4 translation(const glm::vec3& vec)
{
	return
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			vec.x, vec.y, vec.z, 1
		};
}

float normalize(float angleRad)
{
	while (angleRad < 0)
	{
		angleRad += 2.0f * glm::pi<float>();
	}

	while (angleRad >= 2.0f * glm::pi<float>())
	{
		angleRad -= 2.0f * glm::pi<float>();
	}

	return angleRad;
}
