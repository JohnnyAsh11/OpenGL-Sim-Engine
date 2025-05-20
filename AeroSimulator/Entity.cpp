#include "Entity.h"
#include "FileReader.h"
#include "Debug.h"
#include <glm/gtc/type_ptr.hpp>

Entity::Entity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial)
{
	m_pMesh = a_pMesh;
	m_pMaterial = a_pMaterial;
	m_pTransform = new Transform();
}

void Entity::Draw(Camera* a_pCamera)
{
	m_pMaterial->PrepMaterial();

	// Reading the uniforms from the shader and send values.
	GLuint WVP = glGetUniformLocation(m_pMaterial->GetShader()->GetProgramID(), "WVP");
	GLuint WorldInverseTranspose = glGetUniformLocation(m_pMaterial->GetShader()->GetProgramID(), "InverseTransposeWorld");

	// Setting the WVP matrix in the shader.
	GLCall(glUniformMatrix4fv(
		WVP,
		1,
		GL_FALSE,
		glm::value_ptr(
			a_pCamera->GetProjection() * 
			a_pCamera->GetView() *
			m_pTransform->GetWorld())
	));

	// Setting the World Inverse Transpose matrix for the Shader program.
	GLCall(glUniformMatrix4fv(
		WorldInverseTranspose,
		1, 
		GL_FALSE,
		glm::value_ptr(m_pTransform->GetInverseTranspose())
	));

	m_pMesh->Render();
}

Transform* Entity::GetTransform(void) { return m_pTransform; }
std::shared_ptr<Mesh> Entity::GetMesh(void) { return m_pMesh; }
std::shared_ptr<Material> Entity::GetMaterial(void) { return m_pMaterial; }

Entity& Entity::operator=(const Entity& a_pOther)
{
	// Reallocating outstanding memory.
	if (m_pMesh) m_pMesh.reset();
	if (m_pTransform) Realloc(m_pTransform);

	// Setting values.
	m_pMesh = a_pOther.m_pMesh;
	m_pTransform = a_pOther.m_pTransform;

	// Returning a reference to the Entity.
	return *this;
}

Entity::Entity(const Entity& a_pOther)
{
	// Setting values.
	m_pMesh = a_pOther.m_pMesh;
	m_pTransform = a_pOther.m_pTransform;
	m_pMaterial = a_pOther.m_pMaterial;
}

Entity::~Entity()
{
	m_pMesh.reset();
	Realloc(m_pTransform);
}
