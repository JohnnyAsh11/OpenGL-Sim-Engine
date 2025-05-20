#ifndef __SHADER_H_
#define __SHADER_H_

#include <string>
#include <GL/glew.h>

/// <summary>
/// Holds data for a set of Vertex and Fragment shaders in the program.
/// </summary>
class Shader
{
private:
	std::string m_sVertexShaderFile = "";
	std::string m_sFragmentShaderFile = "";		// Equivalent to a pixel shader.
	GLuint m_uProgramID = -1;
	bool m_bIsCompiled = false;

public:
	/// <summary>
	/// Constructs instances of the Shader class.
	/// </summary>
	Shader(void);

	/// <summary>
	/// Copy constructor for Shader objects.
	/// </summary>
	/// <param name="other">Reference to the Shader that is being copied.</param>
	Shader(const Shader& other);

	/// <summary>
	/// Standard destructor for Shader objects.
	/// </summary>
	~Shader(void);

	/// <summary>
	/// Copy operator for the Shader class.
	/// </summary>
	/// <param name="other">Reference to the Shader that is being copied.</param>
	/// <returns>The reference to the new Shader copy.</returns>
	Shader& operator= (const Shader& other);

	/// <summary>
	/// Compiles the Shader into the program.  Should only be called once.
	/// </summary>
	/// <param name="a_sVertexShaderFile">The file path to the vertex shader.</param>
	/// <param name="a_sFragmentShaderFile">The file path to the fragmemt shader.</param>
	/// <param name="a_sProgramName">The name of the program.</param>
	/// <returns>The Program ID.</returns>
	GLuint CompileShader(std::string a_sVertexShaderFile, std::string a_sFragmentShaderFile);

	/// <summary>
	/// Vertex shader field accessor.
	/// </summary>
	/// <returns>The Shader's vertex shader.</returns>
	std::string GetVertexShader();

	/// <summary>
	/// Fragment shader field accessor.
	/// </summary>
	/// <returns>The Shader's fragment shader.</returns>
	std::string GetFragmentShader();

	/// <summary>
	/// Program ID field accessor.  Use this to set the active shader.
	/// </summary>
	/// <returns>The Shader's program ID.</returns>
	int GetProgramID();

	/// <summary>
	/// Gets whether or not the shader program has been compiled for use.
	/// </summary>
	bool IsCompiled();

private:
	/// <summary>
	/// Loads in the shaders from the files and compiles them into usable programs.
	/// </summary>
	/// <param name="a_sVertexShader">Filepath to the vertex shader being used.</param>
	/// <param name="a_sFragmentShader">Filepath to the fragment shader being used.</param>
	/// <returns>The program ID of the shaders.</returns>
	GLuint LoadShaders(const char* a_sVertexShader, const char* a_sFragmentShader);

};

#endif //__SHADER_H_
