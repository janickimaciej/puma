#pragma once

#include <glm/glm.hpp>

glm::vec4 eulerAnglesToQuat(const glm::vec3& eulerAngles);
glm::vec3 quatToEulerAngles(const glm::vec4& quat);
glm::mat4 quatToRotationMatrix(const glm::vec4& quat);
glm::vec4 quatProduct(const glm::vec4& q1, const glm::vec4& q2);
glm::mat4 rotationX(float angleRad);
glm::mat4 rotationY(float angleRad);
glm::mat4 rotationZ(float angleRad);
glm::mat4 translation(const glm::vec3& vec);
float normalize(float angleRad);
