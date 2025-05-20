#include "SkyBox.h"

#include <FreeImage/FreeImage.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Debug.h"

SkyBox::SkyBox(std::shared_ptr<Mesh> a_pCube)
{
    m_pShader = std::make_shared<Shader>(Shader());
    m_pShader->CompileShader(
        "shaders/SkyVertex.glsl",
        "shaders/SkyFrag.glsl");
    m_pCube = std::make_shared<Mesh>(Mesh("models/cube.graphics_obj"));
    m_pCube->CompileMesh();

    m_lFaces.push_back("textures/sky/right.png");
    m_lFaces.push_back("textures/sky/left.png");
    m_lFaces.push_back("textures/sky/up.png");
    m_lFaces.push_back("textures/sky/down.png");
    m_lFaces.push_back("textures/sky/front.png");
    m_lFaces.push_back("textures/sky/back.png");

    // ------------------------------------------------------------------------------------------
    //      Debug Creating the cube vertex buffers and arrays
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // ------------------------------------------------------------------------------------------

    LoadCubeMap();
}

void SkyBox::LoadCubeMap()
{
    // Generating texture IDs and binding the texture.
    glGenTextures(1, &m_dCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_dCubeMap);

    // For each filepath to the faces of the cube map,
    for (GLuint i = 0; i < m_lFaces.size(); i++)
    {
        // Load in the texture with FreeImage.
        FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(m_lFaces[i].c_str(), 0), m_lFaces[i].c_str());

        // Checking if we found the image.
        if (!bitmap) 
        {
            std::cout << "Failed to load image: " << m_lFaces[i] << std::endl;
            continue;
        }

        // Flipping the images because FreeImage assumes to flip for some reason.
        FreeImage_FlipVertical(bitmap);

        // Saving the image as a 32bit image.
        FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);

        // Unloading the originally loaded image since we don't need it anymore.
        FreeImage_Unload(bitmap);

        // Pulling some data from the loaded image for OpenGL.
        int width = FreeImage_GetWidth(bitmap32);
        int height = FreeImage_GetHeight(bitmap32);
        BYTE* data = FreeImage_GetBits(bitmap32);

        // Generating the 2D texture.
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, 
            GL_RGBA,
            width, 
            height, 
            0, 
            GL_BGRA, 
            GL_UNSIGNED_BYTE,
            data);

        // Unloading the 32bit image.
        FreeImage_Unload(bitmap32);
    }

    // OpenGL black magic cube map texture loading.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyBox::Render(Camera* a_Camera)
{
    // Altering the depth function so depth test passes when 
    // values are equal to the depth buffer's content.
    glDepthFunc(GL_LEQUAL);

    // Assigning the program to use the skybox shaders.
    GLCall(glUseProgram(m_pShader->GetProgramID()));

    // Removing the translation aspect of the view matrix.
    glm::mat4 m4View = glm::mat4(glm::mat3(a_Camera->GetView()));

    // Reading the uniforms from the shader and geting their IDs.
    GLuint projection = glGetUniformLocation(m_pShader->GetProgramID(), "projection");
    GLuint view = glGetUniformLocation(m_pShader->GetProgramID(), "view");

    // Sending the uniforms data from the camera.
    GLCall(glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(a_Camera->GetProjection())));
    GLCall(glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(m4View)));
    
    // Binding the skybox VAO and rendering the cubemap with it.
    GLCall(glBindVertexArray(skyboxVAO));
    GLCall(glActiveTexture(GL_TEXTURE0));
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_dCubeMap);
    glDrawArrays(GL_TRIANGLES, 0, m_pCube->GetVertexCount());

    // Reseting values.
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
