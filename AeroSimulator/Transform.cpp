#include "Transform.h"
#include "Math.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext.hpp>

Transform::Transform()
{
	m_bIsDirty = false;
	m_v3Position = VECTOR3_ZERO;
	m_v3Rotation = VECTOR3_ZERO;
	m_v3Scale = glm::vec3(1.0f);
	m_m4World = IDENTITY_M4;
}
Transform::Transform(Transform const& a_pOther)
{
	m_bIsDirty = a_pOther.m_bIsDirty;
	m_v3Position = a_pOther.m_v3Position;
	m_v3Rotation = a_pOther.m_v3Rotation;
	m_v3Scale = a_pOther.m_v3Scale;
	m_m4World = a_pOther.m_m4World;
}
Transform& Transform::operator=(Transform const& a_pOther)
{
	m_bIsDirty = a_pOther.m_bIsDirty;
	m_v3Position = a_pOther.m_v3Position;
	m_v3Rotation = a_pOther.m_v3Rotation;
	m_v3Scale = a_pOther.m_v3Scale;
	m_m4World = a_pOther.m_m4World;

	return *this;
}

glm::vec3 Transform::GetRotation() { return m_v3Rotation; }
glm::vec3 Transform::GetPosition() { return m_v3Position; }
glm::vec3 Transform::GetScale() { return m_v3Scale; }

glm::vec3 Transform::GetUp()
{
	// Constructing the rotation matrix.
	glm::vec3 v3Result = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 ro;
	if (m_v3Rotation.x > 0.0f || m_v3Rotation.x < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.x, AXIS_X);
	}
	if (m_v3Rotation.y > 0.0f || m_v3Rotation.y < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.y, AXIS_Y);
	}
	if (m_v3Rotation.z > 0.0f || m_v3Rotation.z < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.z, AXIS_Z);
	}

	// Creating a quaternion from the rotation matrix.
	glm::quat roQuat = glm::toQuat(ro);

	// Rotating the absolute offset by the quaternion to make the movement relative.
	v3Result = v3Result * roQuat;

	return v3Result;
}
glm::vec3 Transform::GetRight()
{
	// Constructing the rotation matrix.
	glm::vec3 v3Result = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4 ro;
	if (m_v3Rotation.x > 0.0f || m_v3Rotation.x < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.x, AXIS_X);
	}
	if (m_v3Rotation.y > 0.0f || m_v3Rotation.y < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.y, AXIS_Y);
	}
	if (m_v3Rotation.z > 0.0f || m_v3Rotation.z < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.z, AXIS_Z);
	}

	// Creating a quaternion from the rotation matrix.
	glm::quat roQuat = glm::toQuat(ro);

	// Rotating the absolute offset by the quaternion to make the movement relative.
	v3Result = v3Result * roQuat;

	return v3Result;
}
glm::vec3 Transform::GetForward()
{
	// Constructing the rotation matrix.
	glm::vec3 v3Result = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 ro;
	if (m_v3Rotation.x > 0.0f || m_v3Rotation.x < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.x, AXIS_X);
	}
	if (m_v3Rotation.y > 0.0f || m_v3Rotation.y < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.y, AXIS_Y);
	}
	if (m_v3Rotation.z > 0.0f || m_v3Rotation.z < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.z, AXIS_Z);
	}

	// Creating a quaternion from the rotation matrix.
	glm::quat roQuat = glm::toQuat(ro);

	// Rotating the absolute offset by the quaternion to make the movement relative.
	v3Result = v3Result * roQuat;

	return v3Result;
}

void Transform::SetRotation(glm::vec3 a_v3Rotation) 
{ 
	m_v3Rotation = a_v3Rotation; 
	m_bIsDirty = true; 
}
void Transform::SetPosition(glm::vec3 a_v3Position)
{ 
	m_v3Position = a_v3Position;
	m_bIsDirty = true;
}
void Transform::SetScale(glm::vec3 a_v3Scale) 
{ 
	m_v3Scale = a_v3Scale;
	m_bIsDirty = true;
}

void Transform::Rotate(glm::vec3 a_v3Offset)
{
	m_v3Rotation += a_v3Offset;
	m_bIsDirty = true;
}
void Transform::MoveGlobal(glm::vec3 a_v3Offset)
{
	m_v3Position += a_v3Offset;
	m_bIsDirty = true;
}
void Transform::MoveLocal(glm::vec3 a_v3Offset)
{
	m_bIsDirty = true;

	// Constructing the rotation matrix.
	glm::mat4 ro = IDENTITY_M4;
	ro = glm::rotate(ro, m_v3Rotation.z, AXIS_Z);
	ro = glm::rotate(ro, -m_v3Rotation.y, AXIS_Y);
	ro = glm::rotate(ro, -m_v3Rotation.x, AXIS_X);

	// Creating a quaternion from the rotation matrix.
	glm::quat roQuat = glm::quat(ro);

	// Rotating the absolute offset by the quaternion to make the movement relative.
	glm::vec3 v3Result = roQuat * a_v3Offset;

	// Adding the result to the Position.
	m_v3Position += v3Result;
}
void Transform::Scale(glm::vec3 a_v3Offset)
{
	m_v3Scale *= a_v3Offset;
	m_bIsDirty = true;
}

glm::mat4 Transform::GetWorld()
{
	if (!m_bIsDirty) return m_m4World;

	CalculateMatrices();

	return m_m4World;
}

glm::mat4 Transform::GetInverseTranspose()
{
	if (!m_bIsDirty) return m_m4InverseTranspose;

	CalculateMatrices();

	return m_m4InverseTranspose;
}

void Transform::CalculateMatrices(void)
{
	// Constructing the scale and translation matrices.
	glm::mat4 sc = glm::scale(IDENTITY_M4, m_v3Scale);
	glm::mat4 tr = glm::translate(IDENTITY_M4, m_v3Position);
	glm::mat4 ro;

	// Constructing the rotation matrix.
	//		Only do so if the value is non-zero.
	if (m_v3Rotation.x > 0.0f || m_v3Rotation.x < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.x, AXIS_X);
	}
	if (m_v3Rotation.y > 0.0f || m_v3Rotation.y < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.y, AXIS_Y);
	}
	if (m_v3Rotation.z > 0.0f || m_v3Rotation.z < 0.0f)
	{
		ro *= glm::rotate(IDENTITY_M4, m_v3Rotation.z, AXIS_Z);
	}

	// Calculating the world matrix.
	m_m4World = sc * tr * ro;

	// Calculating the transpose inverse matrix.
	m_m4InverseTranspose = glm::transpose(glm::inverse(m_m4World));

	m_bIsDirty = false;
}
