#pragma once
#include <GL/glew.h>
// Macro for assertion, if failed break debug
#define ASSERT(x) if (!(x)) __debugbreak();
// Macro for error checking in opengl 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);