#include "Mesh.h"
#include "Debug.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

// Construction // Rule of Three
Mesh::Mesh()
{
	m_VBO = 0;
	m_VAO = 0;
	m_lVertices = std::vector<Vertex>();
	m_dVertexCount = 0;
}

Mesh::Mesh(const char* a_sFilePath)
{
	m_VBO = 0;
	m_VAO = 0;
	std::fstream modelReader = std::fstream(a_sFilePath);
	m_dVertexCount = 0;

	// ------------------------------------------------------------
	//		Code originally written by Christopher Cascioli,
	//		professor at Rochester Institute of Technology.
	//	
	//	Adapted from DirectX11 to OpenGL/glm by Johnny Fagerlin.
	//	
	//	  Loads in data from .obj files to create mesh objects.
	//	  	     .obj files are renamed to have a 
	//	  	     .graphics_obj file ending for git
	// ------------------------------------------------------------
	// Variables used while reading the file
	m_lVertices = std::vector<Vertex>();	// Verts we're assembling
	std::vector<glm::vec3>  positions;		// Positions from the file
	std::vector<glm::vec3>  normals;		// Normals from the file
	std::vector<glm::vec2> uvs;				// UVs from the file
	char chars[100];						// String for line reading

	// Still have data left?
	while (modelReader.good())
	{
		// Get the line (100 characters should be more than enough)
		modelReader.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			glm::vec3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			glm::vec2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			glm::vec3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			// NOTE: This assumes the given obj file contains
			//  vertex positions, uv coordinates AND normals.
			unsigned int i[12];
			int numbersRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// If we only got the first number, chances are the OBJ
			// file has no UV coordinates.  This isn't great, but we
			// still want to load the model without crashing, so we
			// need to re-read a different pattern (in which we assume
			// there are no UVs denoted for any of the vertices)
			if (numbersRead == 1)
			{
				// Re-read with a different pattern
				numbersRead = sscanf_s(
					chars,
					"f %d//%d %d//%d %d//%d %d//%d",
					&i[0], &i[2],
					&i[3], &i[5],
					&i[6], &i[8],
					&i[9], &i[11]);

				// The following indices are where the UVs should 
				// have been, so give them a valid value
				i[1] = 1;
				i[4] = 1;
				i[7] = 1;
				i[10] = 1;

				// If we have no UVs, create a single UV coordinate
				// that will be used for all vertices
				if (uvs.size() == 0)
					uvs.push_back(glm::vec2(0, 0));
			}

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = glm::vec3(uvs[i[1] - 1], 0.0f);
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = glm::vec2(uvs[i[4] - 1]);
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = glm::vec3(uvs[i[7] - 1], 0.0f);
			v3.Normal = normals[i[8] - 1];

			// The model is most likely in a right-handed space,
			// especially if it came from Maya.  We want to convert
			// to a left-handed space for DirectX.  This means we 
			// need to:
			//  - Invert the Z position
			//  - Invert the normal's Z
			//  - Flip the winding order
			// We also need to flip the UV coordinate since DirectX
			// defines (0,0) as the top left of the texture, and many
			// 3D modeling packages use the bottom left as (0,0)

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Flip Z (LH vs. RH)
			v1.Position.z *= -1.0f;
			v2.Position.z *= -1.0f;
			v3.Position.z *= -1.0f;

			// Flip normal's Z
			v1.Normal.z *= -1.0f;
			v2.Normal.z *= -1.0f;
			v3.Normal.z *= -1.0f;

			// Add the verts to the vector (flipping the winding order)
			m_lVertices.push_back(v1);
			m_lVertices.push_back(v3);
			m_lVertices.push_back(v2);
			m_dVertexCount += 3;

			// Was there a 4th face?
			// - 12 numbers read means 4 faces WITH uv's
			// - 8 numbers read means 4 faces WITHOUT uv's
			if (numbersRead == 12 || numbersRead == 8)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = glm::vec3(uvs[i[10] - 1], 0.0f);
				v4.Normal = normals[i[11] - 1];

				// Flip the UV, Z pos and normal's Z
				v4.UV.y = 1.0f - v4.UV.y;
				v4.Position.z *= -1.0f;
				v4.Normal.z *= -1.0f;

				// Add a whole triangle (flipping the winding order)
				m_lVertices.push_back(v1);
				m_lVertices.push_back(v4);
				m_lVertices.push_back(v3);
				m_dVertexCount += 3;
			}
		}
	}

	modelReader.close();

	// Since we are working with a loaded model, assume it is ready to be compiled immediately.
	CompileMesh();
}

Mesh::~Mesh(void)
{
	// Deleting the Vertex Buffer obj if it exists.
	if (m_VBO > 0)
	{
		glDeleteBuffers(1, &m_VBO);
	}

	// Deleting the Vertex Array obj if it exists.
	if (m_VAO > 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
}
Mesh::Mesh(const Mesh& other)
{
	// Deleting the Vertex Buffer obj if it exists.
	if (m_VBO > 0)
	{
		glDeleteBuffers(1, &m_VBO);
	}

	// Deleting the Vertex Array obj if it exists.
	if (m_VAO > 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	// Instantiating a new list.
	m_lVertices = std::vector<Vertex>();

	// Populate the list with the other Mesh's vertices.
	for (int i = 0; i < other.m_lVertices.size(); i++)
	{
		m_lVertices.push_back((other.m_lVertices)[i]);
	}

	// Setting all other values.
	m_VBO = other.m_VBO;
	m_VAO = other.m_VAO;
	m_dVertexCount = other.m_dVertexCount;

	// Recompiling the mesh after copying the data over.
	CompileMesh();
}

Mesh& Mesh::operator=(const Mesh& other)
{
	// Deleting the Vertex Buffer obj if it exists.
	if (m_VBO > 0)
	{
		glDeleteBuffers(1, &m_VBO);
	}

	// Deleting the Vertex Array obj if it exists.
	if (m_VAO > 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	// Instantiating a new list.
	m_lVertices = std::vector<Vertex>();

	// Populate the list with the other Mesh's vertices.
	for (int i = 0; i < other.m_lVertices.size(); i++)
	{
		m_lVertices.push_back((other.m_lVertices)[i]);
	}

	// Setting all other values.
	m_VBO = other.m_VBO;
	m_VAO = other.m_VAO;
	m_dVertexCount = other.m_dVertexCount;

	// Recompiling the mesh after copying the data over.
	CompileMesh();

	// Returning the altered Mesh.
	return *this;
}

void Mesh::AddVertex(glm::vec3 a_v3VertexPosition)
{
	// Actually adding the vertexc to the list.
	Vertex v = Vertex();
	v.Position = a_v3VertexPosition;
	v.Color = glm::vec3(0.0f);
	v.Normal = glm::vec3(0.0f);
	v.UV = glm::vec3(0.0f);
	m_lVertices.push_back(v);

	// Incrementing the vertex count.
	m_dVertexCount++;
}
void Mesh::AddVertexColor(glm::vec3 a_v3VertexPosition, glm::vec3 a_v3VertexColor)
{
	// Actually adding the vertices to the list.
	Vertex v = Vertex();
	v.Position = a_v3VertexPosition;
	v.Color = a_v3VertexColor;
	v.Normal = glm::vec3(0.0f);
	v.UV = glm::vec3(0.0f);
	m_lVertices.push_back(v);

	// Incrementing the vertex count.
	m_dVertexCount++;
}
void Mesh::Clear(void)
{
	// Actually clearing the list.
	m_lVertices.clear();

	// Resetting variables.
	this->Reset();
}

void Mesh::CompileMesh()
{
	// Creating/Setting the Vertex Array object.
	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));

	// Creating/Setting the Vertex Buffer object.
	GLCall(glGenBuffers(1, &m_VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(
		GL_ARRAY_BUFFER,
		m_dVertexCount * sizeof(Vertex),
		&m_lVertices[0],
		GL_STATIC_DRAW));

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		(GLvoid*)(1 * sizeof(glm::vec3)));

	// UV attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, 
		2,
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		(GLvoid*)(2 * sizeof(glm::vec3)));
	
	// Normal attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Vertex),
		(GLvoid*)(3 * sizeof(glm::vec3)));

	// Unbinding the VAO at the end of the method.
	glBindVertexArray(0);
}

void Mesh::Render()
{
	// Binding this Mesh's VAO.
	GLCall(glBindVertexArray(m_VAO));

	// Drawing the vertex buffers.
	GLCall(glDrawArrays(GL_TRIANGLES, 0, m_dVertexCount));

	// Unbinding the buffers at the end of the method.
	GLCall(glBindVertexArray(0));
}

GLuint Mesh::GetVAO() { return m_VAO; }

int Mesh::GetVertexCount()
{
	return m_dVertexCount;
}

void Mesh::Reset(void)
{
	if (m_VBO > 0)
	{
		glDeleteBuffers(1, &m_VBO);
	}

	if (m_VAO > 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	
	m_dVertexCount = 0;
	m_VAO = 0;
	m_VBO = 0;
}