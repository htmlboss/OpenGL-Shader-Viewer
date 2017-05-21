#pragma once

#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
	enum class Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(50.0f, 25.0f, 50.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float FOV = 45.0f) noexcept;

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(const Camera_Movement direction, const double deltaTime) noexcept;

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void UpdateView(const bool constrainPitch = true) noexcept;

	glm::mat4x4 GetViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }
	glm::mat4x4 GetProjMatrix(const float width, const float height) const { return glm::perspective(m_FOV, width / height, 0.1f, 1000.0f); }
	glm::vec3 GetPosition() const noexcept { return m_position; }

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateVectors();

	// Camera Attributes
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	const glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Eular Angles
	double m_yaw;
	double m_pitch;

	const float m_speed = 50.0f;
	const double m_sensitivity = 0.3;
	const float m_FOV;

	bool m_firstMouse;
	double m_prevX, m_prevY;
};