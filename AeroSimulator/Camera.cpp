#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Math.h"
#include <algorithm>

Camera::Camera(float a_fAspectRatio, float a_fFOV, bool a_bIsOrthographic)
{
	m_fFOV = a_fFOV;
	m_fAspectRatio = a_fAspectRatio;
	m_bIsOrthographic = a_bIsOrthographic;
	m_tTransform = Transform();

	UpdateProjection(a_fAspectRatio);
	UpdateView();
}

Camera::Camera(const Camera& a_pOther)
{
	m_bIsOrthographic = a_pOther.m_bIsOrthographic;
	m_fFOV = a_pOther.m_fFOV;
	m_tTransform = a_pOther.m_tTransform;
	m_m4View = a_pOther.m_m4View;
	m_m4Projection = a_pOther.m_m4Projection;
	m_v2PrevMousePosition = a_pOther.m_v2PrevMousePosition;
	m_fAspectRatio = a_pOther.m_fAspectRatio;
}

Camera& Camera::operator=(const Camera& a_pOther)
{
	m_bIsOrthographic = a_pOther.m_bIsOrthographic;
	m_fFOV = a_pOther.m_fFOV;
	m_tTransform = a_pOther.m_tTransform;
	m_m4View = a_pOther.m_m4View;
	m_m4Projection = a_pOther.m_m4Projection;
	m_v2PrevMousePosition = a_pOther.m_v2PrevMousePosition;
	m_fAspectRatio = a_pOther.m_fAspectRatio;

	return *this;
}

glm::mat4 Camera::GetView()
{
	return m_m4View;
}

glm::mat4 Camera::GetProjection()
{
	return m_m4Projection;
}

Transform& Camera::GetTransform()
{
	return m_tTransform;
}

float Camera::GetFOV()
{
	return m_fFOV;
}

void Camera::UpdateProjection(float a_fAspectRatio)
{
	m_fAspectRatio = a_fAspectRatio;

	if (m_bIsOrthographic)
	{
		// Setting variables for ease.
		float orthoHeight = 0.5f;
		float orthoWidth = orthoHeight * a_fAspectRatio;

		// Creating an orthographic projection matrix.
		m_m4Projection = glm::ortho(
			-orthoWidth / 2.0f,  // Left
			orthoWidth / 2.0f,   // Right
			-orthoHeight / 2.0f, // Bottom
			orthoHeight / 2.0f,  // Top
			NEAR_PLANE,          // Near plane
			FAR_PLANE            // Far plane
		);
	}
	else
	{
		// Creating a perspective projection matrix.
		m_m4Projection = glm::perspective(
			glm::radians(m_fFOV),	// Field of View
			m_fAspectRatio,			// Aspect ratio
			NEAR_PLANE,				// Near plane
			FAR_PLANE				// Far plane
		);
	}
}

void Camera::UpdateView()
{
	// Calculating the target position.
	glm::vec3 v3Target = m_tTransform.GetPosition() + m_tTransform.GetForward();

	// Calculating the view matrix.
	m_m4View = glm::lookAt(m_tTransform.GetPosition(), v3Target, m_tTransform.GetUp());
}

void Camera::Update(float a_fDeltaTime, sf::Window* a_pWindow)
{
	float fSpeed = 0.75f * a_fDeltaTime;

	// Forward and backwards movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_tTransform.MoveLocal(glm::vec3(0.0f, 0.0f, fSpeed));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_tTransform.MoveLocal(glm::vec3(0.0f, 0.0f, -fSpeed));
	}

	// Left and right movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_tTransform.MoveLocal(glm::vec3(fSpeed, 0.0f, 0.0f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_tTransform.MoveLocal(glm::vec3(-fSpeed, 0.0f, 0.0f));
	}
	
	// Up and down movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		m_tTransform.MoveGlobal(glm::vec3(0.0f, fSpeed, 0.0f));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
	{
		m_tTransform.MoveGlobal(glm::vec3(0.0f, -fSpeed, 0.0f));
	}

	// Mouse input checking.
	sf::Vector2i v2CurrentMousePosition = sf::Mouse::getPosition(*a_pWindow);
	sf::Vector2i v2CursorDelta = v2CurrentMousePosition - m_v2PrevMousePosition;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		// Scaling down the delta to be a little more reasonable.
		float fCursorDeltaY = v2CursorDelta.y * 0.0025f;
		float fCursorDeltaX = v2CursorDelta.x * 0.0025f;

		// Not very memory efficient but reduces operations.
		float fRotationX = m_tTransform.GetRotation().x + fCursorDeltaY;	// The future rotation.
		float fMin = (-90.0f * static_cast<float>(PI / 180.0f));			// -1/2PI
		float fMax = (+90.0f * static_cast<float>(PI / 180.0f));			// 1/2PI
		float fPostClamp = std::max(fMin, std::min(fRotationX, fMax));
		
		// Checking if the clamp was done.  If it was, do not rotate around the X axis.
		if (fPostClamp == fMin || fPostClamp == fMax)
		{
			fCursorDeltaY = 0.0f;
		}

		// Rotating with the resulting values.
		m_tTransform.Rotate(glm::vec3(-fCursorDeltaY, fCursorDeltaX, 0.0f));
	}

	UpdateView();
	m_v2PrevMousePosition = v2CurrentMousePosition;
}
