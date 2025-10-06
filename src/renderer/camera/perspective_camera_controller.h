#pragma once

#include "perspective_camera.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class PerspectiveCameraController
{
public:
	PerspectiveCameraController(float fov, float aspect, float nearClip, float farClip);

	void OnUpdate();

	PerspectiveCamera& GetCamera() { return m_Camera; }

private:
	void HandleKeyboardInput(float deltaTime);
	void HandleMouseInput();

private:
	PerspectiveCamera m_Camera;

	float m_CameraSpeed = 5.0f;
	float m_MouseSensitivity = 0.1f;

	glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };
	bool m_FirstMouse = true;
	double m_LastFrameTime = 0.0;
};
