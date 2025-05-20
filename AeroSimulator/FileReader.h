#ifndef __FILEREADER_H_
#define __FILEREADER_H_

#include <string>
#include <GL/glew.h>
#include <GL/wglew.h>

/// <summary>
/// Contains functionality for reading from external files.
/// </summary>
class FileReader
{
private:
	static FileReader* m_pInstance;

public:
	/// <summary>
	/// Retrieves the instance of the FileReader.
	/// </summary>
	/// <returns>The single instance of the FileReader.</returns>
	static FileReader* GetInstance(void);

	/// <summary>
	/// Removes the single instance of the FileReader from memory.
	/// </summary>
	static void ReleaseInstance(void);

	/// <summary>
	/// Reads the contents of an entire file to a single string and returns that string.
	/// </summary>
	/// <param name="a_sfilepath">The filepath to what is being read in.</param>
	/// <returns>Returns the contents of the file as a single string.</returns>
	std::string ReadFile(std::string a_sFilepath = "");

	/// <summary>
	/// Loads in a texture from the passed in filepath.
	/// </summary>
	/// <param name="a_sFilepath">Filepath to the texture.</param>
	/// <returns>The GLuint ID to that texture.</returns>
	GLuint LoadTexture(std::string a_sFilepath);

private:
	/// <summary>
	/// Constructs an instance of the FileReader object.
	/// </summary>
	FileReader(void);

	/// <summary>
	/// Destructs the instances of the FileReader object.
	/// </summary>
	~FileReader(void);

	/// <summary>
	/// Frees the memory allocated by the FileReader object.
	/// </summary>
	static void Release(void);
};

#endif //__FILEREADER_H_

