#ifndef __DEBUG_H_
#define __DEBUG_H_

/// <summary>
/// Checks that there are no errors coming from OpenGL.
/// </summary>
void GLClearError(void);

/// <summary>
/// Logs the OpenGL function calls if they fail as outlined in the macro using GLLogCall.
/// </summary>
/// <param name="a_sFunction">The calling function name.</param>
/// <param name="a_sFile">The file location of the calling function.</param>
/// <param name="a_uLine">The line that the calling function is on.</param>
/// <returns>False if there is an error and true if not.  Slightly unintuitive.</returns>
bool GLLogCall(const char* a_sFunction, const char* a_sFile, int a_uLine);

/* Helper macro for the GLCall macro. If the passed in value evaluates to
   false, it will halt execution at that line of code. */
#define ASSERT(x) if (!(x)) __debugbreak();

/* Error handling macro for GL function calls. */
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* Safely reallocates memory.  Deletes data and initializes the pointer to nullptr. */
#define Realloc(p) { if (p) { delete p; p = nullptr; } }

#endif //__DEBUG_H_