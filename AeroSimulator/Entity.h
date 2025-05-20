#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "Camera.h"
#include "Debug.h"

/// <summary>
/// Container class for Mesh and Transform objects.
/// </summary>
class Entity
{
private:
	std::shared_ptr<Material> m_pMaterial = nullptr;
	std::shared_ptr<Mesh> m_pMesh = nullptr;
	Transform* m_pTransform = nullptr;

public:
	/// <summary>
	/// Constructs an instance of the entity class.
	/// </summary>
	/// <param name="a_pMesh">Mesh </param>
	Entity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial);

	/// <summary>
	/// Renders the Entity's Mesh using the passed in Camera and Transform field.
	/// </summary>
	/// <param name="a_pCamera">The active Camera for the application.</param>
	void Draw(Camera* a_pCamera);

	/// <summary>
	/// Gets a pointer to the Entity's Transform.
	/// </summary>
	Transform* GetTransform(void);

	/// <summary>
	/// Gets the Entity's Mesh.
	/// </summary>
	std::shared_ptr<Mesh> GetMesh(void);

	/// <summary>
	/// Gets the Entity's set Material.
	/// </summary>
	std::shared_ptr<Material> GetMaterial(void);

	/// <summary>
	/// Copy operator for the Entity class.
	/// </summary>
	/// <param name="a_pOther">The Entity that this one is copying from.</param>
	/// <returns>A deep copy of the original Entity.</returns>
	Entity& operator=(const Entity& a_pOther);

	/// <summary>
	/// Constructs a deep copy of the passed in Entity.
	/// </summary>
	/// <param name="a_pOther">The Entity being copied from.</param>
	Entity(const Entity& a_pOther);

	/// <summary>
	/// Destructs instances of Entities.
	/// </summary>
	~Entity();

};

#endif //__ENTITY_H_
