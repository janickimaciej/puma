#pragma once

#include "mesh.hpp"
#include "model.hpp"

#include <glm/glm.hpp>

#include <memory>

class Box : public Model
{
public:
	Box(const glm::vec3& color);
	virtual ~Box() = default;

	virtual void render() const override;

private:
	std::shared_ptr<Mesh> m_mesh{};
	glm::vec3 m_color{};

	void updateShaders() const;
};
