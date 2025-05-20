#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>

#include "Shader.h"

/// <summary>
/// Manages a set of shaders and handles uniforms for those shaders.
/// </summary>
class Material
{
private:
	std::shared_ptr<Shader> m_pShader;
	glm::vec2 m_v2Offset;
	glm::vec2 m_v2Scale;
	float m_fRoughness;

	std::unordered_map<std::string, GLuint> m_mTextures;
public:
	/// <summary>
	/// Constructs a Material with the passed in Shader and roughness value.
	/// </summary>
	/// <param name="a_pShader">Shader set used by this Material.</param>
	/// <param name="a_fRoughness">The roughness value used by this Material.</param>
	Material(std::shared_ptr<Shader> a_pShader, float a_fRoughness);

	/// <summary>
	/// Retrieves the Shader used by this Material.
	/// </summary>
	std::shared_ptr<Shader> GetShader(void);
	
	/// <summary>
	/// Adds a texture to the texture map from the specified filepath.
	/// </summary>
	/// <param name="a_sFilepath">Filepath to the texture.</param>
	/// <param name="a_sUniformName">Name of the associated uniform in shader code.</param>
	void AddTextureFromFile(std::string a_sFilepath, std::string a_sUniformName);

	/// <summary>
	/// Adds a Uniform Name and Texture ID key value pair to the textures map.
	/// </summary>
	/// <param name="a_sUniformName">The exact name of the uniform in the Shaders.</param>
	/// <param name="a_dTextureID">The ID of the texture in GPU memory.</param>
	void AddTexture(std::string a_sUniformName, GLuint a_dTextureID);

	/// <summary>
	/// Sets all of the textures for upcoming render calls.
	/// </summary>
	void PrepMaterial();
};

#endif //__MATERIAL_H_
