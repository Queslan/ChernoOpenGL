#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout {
public:
	VertexBufferLayout();;

	template<typename T>

	void Push(unsigned int count);

	const std::vector<VertexBufferElement>& GetElements() const;
	unsigned int GetStride() const;
private:
	std::vector <VertexBufferElement> m_Elemements;
	unsigned int m_Stride;
};

