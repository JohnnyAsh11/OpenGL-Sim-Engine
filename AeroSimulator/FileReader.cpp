#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <FreeImage/FreeImage.h>
#include "Debug.h"

#define NULL_STR ""

FileReader* FileReader::m_pInstance = nullptr;

FileReader::~FileReader(void) { Release(); }

FileReader::FileReader(void) {}

FileReader* FileReader::GetInstance(void)
{
	// Instantiating the single instance of the FileReader.
	if (m_pInstance == nullptr)
	{
		m_pInstance = new FileReader();
	}

	return m_pInstance;
}

void FileReader::Release(void)
{
	// Reallocate any memory here.
}

void FileReader::ReleaseInstance(void)
{
	// If there is an instance of the FileReader:
	if (m_pInstance != nullptr)
	{
		// Reallocate the memory.
		Realloc(m_pInstance);
	}
}

std::string FileReader::ReadFile(std::string a_sFilepath)
{
	if (a_sFilepath == NULL_STR)
	{
		return NULL_STR;
	}

	// Initializing the stream reader.
	std::string fileContent;
	std::ifstream reader(a_sFilepath, std::ios::in);

	// Checking that the file was successfully opened.
	if (reader.is_open())
	{
		std::string Line = "";

		// Compiling all of the lines in the single string.
		while (getline(reader, Line))
		{
			fileContent += "\n" + Line;
		}

		// Closing the file.
		reader.close();
	}
	else
	{
		std::cout << "There was an error opening the file." << std::endl;
	}

	return fileContent;
}

GLuint FileReader::LoadTexture(std::string a_sFilepath)
{
	// Figuring out the file format.
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(a_sFilepath.c_str(), 0);

	// If an unknown file type, return a default value.
	if (fif == FIF_UNKNOWN)
	{
		return 0;
	}

	// Load in the actual bitmap.
	FIBITMAP* bitmap = FreeImage_Load(fif, a_sFilepath.c_str());
	if (!bitmap)
		return 0;

	// Loading in the bitmap in 32 bit for proper data format.
	FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);

	// Unloading the original since it is unneeded.
	FreeImage_Unload(bitmap);

	// Getting the width, height and individual pixels of the image.
	int width = FreeImage_GetWidth(bitmap32);
	int height = FreeImage_GetHeight(bitmap32);
	BYTE* pixels = FreeImage_GetBits(bitmap32);

	// Allocating an ID for the loaded image.
	GLuint textureID;
	GLCall(glGenTextures(1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

	// Uploading the image to the GPU with the new ID.
	GLCall(glTexImage2D(
		GL_TEXTURE_2D,				// The type of texture.
		0,							// Base image level.
		GL_RGBA,					// Internal GL storage type.
		width,						// Width of the texture.
		height,						// Height of the texture.
		0,							// Always 0.  Depracated GL feature.
		GL_BGRA,					// Format of the incoming data.
		GL_UNSIGNED_BYTE,			// The type of each color component.
		pixels));					// Pointer to the actual pixel data.

	// Set the parameters of the texture properly.
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// Unload the second and final bitmap.
	FreeImage_Unload(bitmap32);

	// Return the ID for the created texture.
	return textureID;
}