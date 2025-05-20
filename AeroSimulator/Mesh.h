#ifndef __MESH_H_
#define __MESH_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Shader.h"

/// <summary>
/// Container struct to hold data for individual vertices.
/// </summary>
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 UV;
	glm::vec3 Normal;
};

/// <summary>
/// Contains the data necessary for models and primitive objects.
/// </summary>
class Mesh
{
private:
	GLuint m_VBO;
	GLuint m_VAO;
	std::vector<Vertex> m_lVertices;
	int m_dVertexCount;

public:
	/// <summary>
	/// Constructs an instance of the Mesh object.
	/// </summary>
	Mesh(void);

	/// <summary>
	/// Constructs a Mesh from a passed in filepath to a graphics_obj file.
	/// </summary>
	/// <param name="a_sFilePath">Path to the models used in the simulation.</param>
	/// <param name="a_pShader">Preloaded shader for the Mesh to use.</param>
	Mesh(const char* a_sFilePath);

	/// <summary>
	/// Mesh class destructor.
	/// </summary>
	~Mesh(void);

	/// <summary>
	/// Copy constructor for the Mesh copy constructor.
	/// </summary>
	/// <param name="other">Reference to the Mesh being copied.</param>
	Mesh(const Mesh& other);

	/// <summary>
	/// Copy operator for the Mesh class.
	/// </summary>
	/// <param name="other">The other Mesh object being copied.</param>
	/// <returns>A reference to the new Mesh object.</returns>
	Mesh& operator= (const Mesh& other);

	/// <summary>
	/// Adds a Vertex position to the list of vertices.
	/// </summary>
	/// <param name="a_v3VertexPosition">Vector3 position of the vertex.</param>
	void AddVertex(glm::vec3 a_v3VertexPosition);

	/// <summary>
	/// Adds a Vertex color and position to the list of vertices.
	/// </summary>
	/// <param name="a_v3VertexPosition">The Vector3 position of the Vertex.</param>
	/// <param name="a_v3VertexColor">The Vector3 RGB color of the Vertex.</param>
	void AddVertexColor(glm::vec3 a_v3VertexPosition, glm::vec3 a_v3VertexColor);

	/// <summary>
	/// Clears the list of vertices and resets member fields.
	/// </summary>
	void Clear(void);

	/// <summary>
	/// Calls all necessary OpenGL functionalities to set this Mesh's buffers.
	/// </summary>
	void CompileMesh(void);

	/// <summary>
	/// Renders this Mesh's buffers to the window.
	/// </summary>
	void Render();

	/// <summary>
	/// Retrieves the VAO buffer.  Primarily for SkyBox rendering.
	/// </summary>
	GLuint GetVAO();

	/// <summary>
	/// Gets the number of vertices inside of the mesh.
	/// </summary>
	int GetVertexCount();

private:

	/// <summary>
	/// Resets the vbo and vao objects in addition to the enum flag.
	/// </summary>
	void Reset(void);
};

#endif //__MESH_H_
