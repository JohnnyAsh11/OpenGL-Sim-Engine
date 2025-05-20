#ifndef __SKYBOX_H_
#define __SKYBOX_H_

#include <GL/glew.h>
#include <GL/wglew.h>
#include <vector>
#include <memory>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

/// <summary>
/// Handles all data for rendering a SkyBox.
/// </summary>
class SkyBox
{
private:
    std::shared_ptr<Mesh> m_pCube = nullptr;
	std::shared_ptr<Shader> m_pShader = nullptr;
	std::vector<std::string> m_lFaces;
	GLuint m_dCubeMap;

#pragma region Debug while figuring out Mesh class issues.
    float skyboxVertices[108] = {        
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    unsigned int skyboxVAO;
    unsigned int skyboxVBO;
#pragma endregion

public:
	/// <summary>
	/// Constructs a SkyBox for the world.
	/// </summary>
	/// <param name="a_pCube">Cube model being used for the SkyBox.</param>
	SkyBox(std::shared_ptr<Mesh> a_pCube);

	/// <summary>
	/// Renders the SkyBox to the game world.
	/// </summary>
	/// <param name="a_Camera">Active Camera being used for view and projection matrices.</param>
	void Render(Camera* a_Camera);

private:
	/// <summary>
	/// Helper method to load in the cube map.
	/// </summary>
	void LoadCubeMap();
};

#endif //__SKYBOX_H_
