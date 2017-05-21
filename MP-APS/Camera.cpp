#include "Camera.h"
#include "Input.h"

/***********************************************************************************/
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float FOV) noexcept : 
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	m_FOV(FOV),
	m_position(position),
	m_yaw(yaw),
	m_pitch(pitch),
	m_firstMouse(true),
	m_prevX(0.0),
	m_prevY(0.0) {

	updateVectors();
}

/***********************************************************************************/
void Camera::ProcessKeyboard(const Camera_Movement direction, const double deltaTime) noexcept {
	const float velocity = m_speed * static_cast<float>(deltaTime);

	switch (direction) {
	case Camera_Movement::FORWARD:
		m_position += m_front * velocity;
		break;
	case Camera_Movement::BACKWARD:
		m_position -= m_front * velocity;
		break;
	case Camera_Movement::LEFT:
		m_position -= m_right * velocity;
		break;
	case Camera_Movement::RIGHT:
		m_position += m_right * velocity;
		break;
	case Camera_Movement::UP:
		m_position += m_worldUp * velocity;
		break;
	case Camera_Movement::DOWN:
		m_position -= m_worldUp * velocity;
		break;
	}
}

/***********************************************************************************/
void Camera::UpdateView(const bool constrainPitch /*= true*/) noexcept {
	
	const auto xPos = Input::GetInstance()->GetMouseX();
	const auto yPos = Input::GetInstance()->GetMouseY();
	
	// If the mouse position has changed, recalculate vectors
	if (xPos != m_prevX || yPos != m_prevY) {
		if (m_firstMouse) {
			m_prevX = xPos;
			m_prevY = yPos;
			m_firstMouse = false;
		}

		auto xOffset = (xPos - m_prevX) * m_sensitivity;
		auto yOffset = (m_prevY - yPos) * m_sensitivity; // Reversed since y-coordinates go from bottom to top

		m_prevX = xPos;
		m_prevY = yPos;

		m_yaw += xOffset;
		m_pitch += yOffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (m_pitch > 89.0f) {
				m_pitch = 89.0f;
			}
			if (m_pitch < -89.0f) {
				m_pitch = -89.0f;
			}
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateVectors();
	}
}

/***********************************************************************************/
void Camera::updateVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
