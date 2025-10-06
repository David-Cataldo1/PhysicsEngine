#pragma once

#include <stack>

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera() {}
	PerspectiveCamera(float fovy, float aspect, float zNear, float zFar);

	void SetProjection(float fovy, float aspect, float zNear, float zFar);

	void SetPosition(const glm::vec3& pos) { m_Position = pos; CalculateViewMatrix(); }

	void Move(float dist, const glm::vec3& cameraAxis);
	void Rotate(float xAngle, float yAngle);

	glm::vec3 GetForwardVector() const { return m_ForwardVector; }

	virtual glm::vec3 GetPosition() const override { return m_Position; }
	virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
	virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
	virtual const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

	virtual ~PerspectiveCamera() override {}


private:
	virtual void CalculateViewMatrix() override;
	void CalcForwardVector();

private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::vec3 m_ForwardVector = glm::vec3(0.0f, 0.0f, 1.0f);

	float yaw;
	float pitch;
};
