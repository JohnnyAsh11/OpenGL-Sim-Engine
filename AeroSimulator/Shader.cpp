#include "Shader.h"
#include "Debug.h"
#include "FileReader.h"
#include <iostream>

#define NULL_STR ""
#define ERROR 0

Shader::Shader(void)
{
	m_sFragmentShaderFile = NULL_STR;
	m_sVertexShaderFile = NULL_STR;
	m_uProgramID = -1;
}

Shader::Shader(const Shader& a_pOther)
{
	// Setting all of the variables.
	m_sFragmentShaderFile = a_pOther.m_sFragmentShaderFile;
	m_sVertexShaderFile = a_pOther.m_sVertexShaderFile;
	m_uProgramID = a_pOther.m_uProgramID;
	
	// Explicitly not copying the compiled bool.
	//m_bIsCompiled = a_pOther.m_bIsCompiled;
}

Shader::~Shader(void)
{
	if (glIsProgram(m_uProgramID))
	{
		// Deleting the shader.
		GLCall(glDeleteProgram(m_uProgramID));

		// Make sure we are not using this Shader.
		GLCall(glUseProgram(0));

		// Setting the Program ID to a faux value.
		m_uProgramID = -1;
	}
}

Shader& Shader::operator=(const Shader& a_pOther)
{
	// Setting all of the variables.
	m_sFragmentShaderFile = a_pOther.m_sFragmentShaderFile;
	m_sVertexShaderFile = a_pOther.m_sVertexShaderFile;
	m_uProgramID = a_pOther.m_uProgramID;

	// Explicitly not copying the compiled bool.
	//m_bIsCompiled = a_pOther.m_bIsCompiled;

	return *this;
}

GLuint Shader::CompileShader(
	std::string a_sVertexShaderFile,
	std::string a_sFragmentShaderFile)
{
	// If it's already compiled, just return the program ID.
	if (m_bIsCompiled) return m_uProgramID;

	// Setting the shader file addresses.
	m_sVertexShaderFile = a_sVertexShaderFile;
	m_sFragmentShaderFile = a_sFragmentShaderFile;

	// Loading the shaders and getting their program ID
	m_uProgramID = LoadShaders(
		m_sVertexShaderFile.c_str(),
		m_sFragmentShaderFile.c_str()
	);

	// The shader has finished compilation.
	m_bIsCompiled = true;

	// Returning the program ID.
	return m_uProgramID;
}

// - - Accessors - -
std::string Shader::GetVertexShader() { return m_sVertexShaderFile; }
std::string Shader::GetFragmentShader() { return m_sFragmentShaderFile; }
int Shader::GetProgramID() { return m_uProgramID; }
bool Shader::IsCompiled() { return m_bIsCompiled; }


// - - Private Methods - -
GLuint Shader::LoadShaders(const char* a_sVertexShader, const char* a_sFragmentShader)
{
	// Creating the shaders.
	GLuint uVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint uFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	std::string sVertexCode, sFragmentCode;

	// Getting a pointer to the FileReader singleton.
	FileReader* pFileReader = FileReader::GetInstance();

	// Reading in the shader code from the files.
	sVertexCode = pFileReader->ReadFile(a_sVertexShader);
	sFragmentCode = pFileReader->ReadFile(a_sFragmentShader);

	// Returning an error if either shader is an empty string.
	if (sVertexCode == "" || sFragmentCode == "")
	{
		return ERROR;
	}

	// Compiling the vertex shader:
	std::cout << "\n\tVertex: " << a_sVertexShader << ":\n" << sVertexCode << std::endl;
	char const* sVertexSource = sVertexCode.c_str();
	GLCall(glShaderSource(uVertexShaderID, 1, &sVertexSource, NULL));
	GLCall(glCompileShader(uVertexShaderID));

	// Compiling the fragment shader:
	std::cout << "\n\tFragment: " << a_sFragmentShader << ":\n" << sFragmentCode << std::endl;
	char const* sFragmentSource = sFragmentCode.c_str();
	GLCall(glShaderSource(uFragmentShaderID, 1, &sFragmentSource, NULL));
	GLCall(glCompileShader(uFragmentShaderID));

	// Linking the shaders to the program.
	GLuint uProgramID = glCreateProgram();
	GLCall(glAttachShader(uProgramID, uVertexShaderID));
	GLCall(glAttachShader(uProgramID, uFragmentShaderID));
	GLCall(glLinkProgram(uProgramID));

	GLCall(glDeleteShader(uFragmentShaderID));
	GLCall(glDeleteShader(uVertexShaderID));

	return uProgramID;
}
