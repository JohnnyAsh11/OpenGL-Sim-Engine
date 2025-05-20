#ifndef __TRANSFORM_H_
#define __TRANSFORM_H_

#include <glm/glm.hpp>

/// <summary>
/// Tracks all transformations performed on a single instance of this object in the world.
/// </summary>
struct Transform
{
private:
	bool m_bIsDirty;
	glm::vec3 m_v3Rotation;
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Scale;
	glm::mat4 m_m4World;
	glm::mat4 m_m4InverseTranspose;

public:
	// - - Construction - -	
	/// <summary>
	/// Default constructor for the Transform struct, initializes all fields.
	/// </summary>
	Transform();

	/// <summary>
	/// Copy constructor for the Transform struct.
	/// </summary>
	/// <param name="a_pOther">The Transform being copied.</param>
	Transform(Transform const& a_pOther);

	/// <summary>
	/// The copy operator overload for the Transform struct.
	/// </summary>
	/// <param name="a_pOther">The Transform being copied.</param>
	/// <returns>A reference to the new Transform.</returns>
	Transform& operator=(Transform const& a_pOther);

	// - - Get Accessors - -
	/// <summary>
	/// Gets the Vector3 rotation.
	/// </summary>
	glm::vec3 GetRotation();

	/// <summary>
	/// Gets the Vector3 position.
	/// </summary>
	glm::vec3 GetPosition();

	/// <summary>
	/// Gets the Vector3 scale.
	/// </summary>
	glm::vec3 GetScale();

	/// <summary>
	/// Gets or potentially recalculates the world matrix.
	/// </summary>
	glm::mat4 GetWorld();

	/// <summary>
	/// Gets the Inverse Transpose of the world matrix.
	/// </summary>
	/// <returns></returns>
	glm::mat4 GetInverseTranspose();
	
	/// <summary>
	/// Gets the up direction of the Transform.
	/// </summary>
	glm::vec3 GetUp();

	/// <summary>
	/// Gets the right direction of the Transform.
	/// </summary>
	glm::vec3 GetRight();

	/// <summary>
	/// Gets the forward direction of the Transform.
	/// </summary>
	/// <returns></returns>
	glm::vec3 GetForward();

	// - - Set Accessors - -
	/// <summary>
	/// Sets the rotation to the passed in Vector3.
	/// </summary>
	void SetRotation(glm::vec3 a_v3Rotation);

	/// <summary>
	/// Sets the position to the passed in Vector3.
	/// </summary>
	void SetPosition(glm::vec3 a_v3Position);

	/// <summary>
	/// Sets the scale to the passed in Vector3.
	/// </summary>
	void SetScale(glm::vec3 a_v3Scale);
	
	// - - Move Operations - -
	 /// <summary>
	 /// Rotates the Transform by the passed in offset.
	 /// </summary>
	 void Rotate(glm::vec3 a_v3Offset);

	 /// <summary>
	 /// Translates the Transform by the passed in offset relative to the global coordinate system.
	 /// </summary>
	 void MoveGlobal(glm::vec3 a_v3Offset);

	 /// <summary>
	 /// Translates the Transform by the passed in offset relative to the local coordinate system.
	 /// </summary>
	 void MoveLocal(glm::vec3 a_v3Offset);

	 /// <summary>
	 /// Scales the Transform by the passed in offset.
	 /// </summary>
	 void Scale(glm::vec3 a_v3Offset);

private:
	/// <summary>
	/// Calculates the world and inverse transposes for this Transform.
	/// </summary>
	void CalculateMatrices(void);
};

#endif //__TRANSFORM_H_
