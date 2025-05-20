#include "Material.h"
#include "FileReader.h"
#include "Debug.h"

Material::Material(std::shared_ptr<Shader> a_pShader, float a_fRoughness)
{
	m_pShader = a_pShader;
	m_fRoughness = a_fRoughness;
}

std::shared_ptr<Shader> Material::GetShader() { return m_pShader; }

void Material::AddTextureFromFile(std::string a_sFilepath, std::string a_sUniformName)
{
	// Loading in the texture from the address passed in.
	GLuint textureID = FileReader::GetInstance()->LoadTexture(a_sFilepath);

	// Inserting it into the hash table.
	m_mTextures.insert({ a_sUniformName, textureID });
}

void Material::AddTexture(std::string a_sUniformName, GLuint a_dTextureID)
{
	// Inserting both values into the hash table.
	m_mTextures.insert({ a_sUniformName, a_dTextureID });
}

void Material::PrepMaterial()
{	
	int dTextureUnit = 0;

	// Assigning the program to use this Mesh's Shaders.
	GLCall(glUseProgram(m_pShader->GetProgramID()));
	
	// Looping through all textures.
	for (const auto& t : m_mTextures) 
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + dTextureUnit));

		// Binding the texture and setting it in the Shader program.
		GLCall(glBindTexture(GL_TEXTURE_2D, t.second));
		GLCall(glUniform1i(glGetUniformLocation(m_pShader->GetProgramID(), t.first.c_str()), dTextureUnit));

		dTextureUnit++;
	}
}
