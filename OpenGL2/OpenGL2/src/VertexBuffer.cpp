#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {

	GLCall(glGenBuffers(1, &m_RenderID)); // Generate buffer object names
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID)); // Bind a named buffer object
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Creates and initializes a buffer object's data store

}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RenderID))
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID)); // Bind a named buffer object
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Bind a named buffer object
}
