#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float zNear, float zFar)
	: m_ProjectionMatrix(glm::perspective(glm::radians(fovy), aspect, zNear, zFar)), m_ViewMatrix(1.0f), yaw(-90.0f), pitch(0.0f), m_Position(0.0f, 14.0f, 27.0f)
{
	CalcForwardVector();
	CalculateViewMatrix();
}

void PerspectiveCamera::Move(float dist, const glm::vec3& cameraAxis)
{
	glm::vec3 normalized = glm::normalize(cameraAxis);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Forwards / Backwards
	m_Position += m_ForwardVector * dist * normalized.z;
	// Up / Down
	m_Position += worldUp * dist * normalized.y;
	// Left / Right - CameraRightVector
	m_Position += glm::cross(worldUp, m_ForwardVector) * dist * -normalized.x;

	CalculateViewMatrix();
}

void PerspectiveCamera::Rotate(float xAngle, float yAngle)
{
	yaw += yAngle;
	pitch -= xAngle;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	CalcForwardVector();
	CalculateViewMatrix();
}

void PerspectiveCamera::CalcForwardVector()
{
	m_ForwardVector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_ForwardVector.y = sin(glm::radians(pitch));
	m_ForwardVector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_ForwardVector = glm::normalize(m_ForwardVector);
}

void PerspectiveCamera::SetProjection(float fovy, float aspect, float zNear, float zFar)
{
	m_ProjectionMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::CalculateViewMatrix()
{
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(m_ForwardVector, worldUp));
	// Calcs and normalizes first the right vector, then the up vector
	glm::vec3 up = glm::normalize(glm::cross(right, m_ForwardVector));


	//glm::vec3 up = glm::normalize(glm::cross(glm::normalize(glm::cross(m_ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f))), m_ForwardVector));
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_ForwardVector, up);

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
