#include "PerspectiveCameraController.h"
#include "Events/Input.h"

PerspectiveCameraController::PerspectiveCameraController(float fov, float aspect, float nearClip, float farClip)
	: m_Camera(fov, aspect, nearClip, farClip)
{
	m_LastFrameTime = glfwGetTime();
}

void PerspectiveCameraController::OnUpdate()
{
	double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - m_LastFrameTime);
	m_LastFrameTime = currentTime;

	HandleKeyboardInput(deltaTime);
	HandleMouseInput();
}

void PerspectiveCameraController::HandleKeyboardInput(float deltaTime)
{
	glm::vec3 direction(0.0f);

	if (Input::IsKeyDown(GLFW_KEY_W)) direction.z += 1.0f;
	if (Input::IsKeyDown(GLFW_KEY_S)) direction.z -= 1.0f;
	if (Input::IsKeyDown(GLFW_KEY_A)) direction.x -= 1.0f;
	if (Input::IsKeyDown(GLFW_KEY_D)) direction.x += 1.0f;
	if (Input::IsKeyDown(GLFW_KEY_SPACE)) direction.y += 1.0f;
	if (Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) direction.y -= 1.0f;

	if (glm::length(direction) > 0.0f)
		m_Camera.Move(m_CameraSpeed * deltaTime * 5, direction);
}

void PerspectiveCameraController::HandleMouseInput()
{

	if (Input::IsMouseDown(GLFW_MOUSE_BUTTON_LEFT))
	{



		glm::vec2 currentMouse = Input::GetMousePos();

		//glm::vec2 currentMouse(static_cast<float>(pos.x), static_cast<float>(pos.y));

		if (m_FirstMouse)
		{
			m_LastMousePosition = currentMouse;
			m_FirstMouse = false;
			return;
		}

		glm::vec2 offset = (currentMouse - m_LastMousePosition) * m_MouseSensitivity;
		m_LastMousePosition = currentMouse;

		m_Camera.Rotate(offset.y, offset.x);  // pitch, yaw
	}
	else
	{
		m_FirstMouse = true;
	}
}
