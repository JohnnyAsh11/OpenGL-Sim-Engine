#include "Debug.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <iostream>

void GLClearError(void)
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* a_sFunction, const char* a_sFile, int a_uLine)
{
    while (GLenum error = glGetError())
    {
        // Breaking in to this line and viewing the value of error as
        // a hexidecimal will allow you to get the correct value to search for
        // in GLEW Header files.
        std::cout << "OPENGL ERROR (" << error << "): " << a_sFunction <<
            " " << a_sFile << ":" << a_uLine << std::endl;
        return false;
    }

    return true;
}
