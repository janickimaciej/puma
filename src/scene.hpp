#pragma once

#include "camera/perspectiveCamera.hpp"
#include "framebuffer.hpp"
#include "frame.hpp"
#include "interpolation.hpp"
#include "plane/plane.hpp"
#include "robot.hpp"
#include "quad.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

class Scene
{
public:
	Scene(const glm::ivec2& viewportSize);
	void update();
	void render();
	void updateViewportSize();

	void addPitchCamera(float pitchRad);
	void addYawCamera(float yawRad);
	void moveXCamera(float x);
	void moveYCamera(float y);
	void zoomCamera(float zoom);

	void updateCameraGUI();

	void startInterpolation();
	void stopInterpolation();
	void resetInterpolation();

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

	float getAnimationTime() const;
	void setAnimationTime(float time);
	int getIntermediateFrameCount() const;
	void setIntermediateFrameCount(int count);
	bool getRenderIntermediateFrames() const;
	void setRenderIntermediateFrames(bool render);
	float getTime() const;

private:
	const glm::ivec2& m_viewportSize{};
	PerspectiveCamera m_camera;

	std::unique_ptr<Framebuffer> m_leftFramebuffer;
	std::unique_ptr<Framebuffer> m_rightFramebuffer;
	Quad m_quad{};

	static constexpr float gridScale = 5.0f;
	Plane m_plane{gridScale};

	int m_intermediateFrameCount = 30;

	Frame m_configFrame{false};
	std::vector<Frame> m_configFrames;
	Robot m_configRobot;
	Frame m_quatFrame{false};
	std::vector<Frame> m_quatFrames;
	Robot m_quatRobot;

	Interpolation m_interpolation;
	bool m_renderIntermediateFrames = false;

	void setUpFramebuffer() const;
	void clearFramebuffer() const;

	void renderGrid() const;
	void renderFrames(const Frame& mainFrame, const std::vector<Frame>& intermediateFrames) const;
};
