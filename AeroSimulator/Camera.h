#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <SFML/Window.hpp>
#include <glm/glm.hpp>

#include "Transform.h"

#define NEAR_PLANE 0.01f
#define FAR_PLANE 100.0f

/// <summary>
/// Defines the view and projection matrices for 3D rendering.
/// </summary>
class Camera
{
private:
	bool m_bIsOrthographic;
	float m_fFOV;
	float m_fAspectRatio;
	Transform m_tTransform;
	glm::mat4 m_m4View;
	glm::mat4 m_m4Projection;
	sf::Vector2i m_v2PrevMousePosition;

public:
	/// <summary>
	/// Constructs the Camera object with a starting FOV and Aspect Ratio 
	/// for the view and projection matrices.
	/// </summary>
	/// <param name="a_fAspectRatio">The aspect ratio of the window.</param>
	/// <param name="a_fFOV">The field of view of this camera.</param>
	/// <param name="a_bIsOrthographic">Determines whether or not the camera is orthographic.</param>
	Camera(float a_fAspectRatio, float a_fFOV, bool a_bIsOrthographic = false);

	/// <summary>
	/// Copy constructor for the Camera class.
	/// </summary>
	Camera(const Camera& a_pOther);

	/// <summary>
	/// Copy operator overload for the Camera class.
	/// </summary>
	Camera& operator=(const Camera& a_pOther);

	/// <summary>
	/// Gets the current view matrix of the camera.
	/// </summary>
	glm::mat4 GetView();

	/// <summary>
	/// Gets the current projection matrix of the camera.
	/// </summary>
	glm::mat4 GetProjection();

	/// <summary>
	/// Gets the Camera's Transform for other classes to manipulate its positions.
	/// </summary>
	Transform& GetTransform();

	/// <summary>
	/// Gets the Field of View of the Camera.
	/// </summary>
	float GetFOV();

	/// <summary>
	/// Updates the projection matrix if the aspect ratio of the window changed.
	/// </summary>
	void UpdateProjection(float a_fAspectRatio);

	/// <summary>
	/// Updates the view matrix of the camera.
	/// </summary>
	void UpdateView();	

	/// <summary>
	/// Updates the position movement of the Camera.
	/// </summary>
	void Update(float a_fDeltaTime, sf::Window* a_pWindow);
};

#endif //__CAMERA_H_