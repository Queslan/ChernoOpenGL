#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Macro for assertion, if failed break debug
#define ASSERT(x) if (!(x)) __debugbreak();
// Macro for error checking in opengl 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};