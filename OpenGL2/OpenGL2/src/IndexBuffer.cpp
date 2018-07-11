#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
:m_Count(count) {

	GLCall(glGenBuffers(1, &m_RenderID)); // Generate buffer object names
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID)); // Bind a named buffer object
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); // Creates and initializes a buffer object's data store

}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RenderID))
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID)); // Bind a named buffer object
}

void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // Bind a named buffer object
}
