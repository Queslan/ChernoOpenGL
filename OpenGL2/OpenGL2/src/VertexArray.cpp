#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererId)); // generate vertex array object names
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererId));
}


void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(0)); // Enable a generic vertex attribute array
		 // This bind Vertex Array with Vertex Buffer that is binded right now 
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, 
			layout.GetStride(), (const void*)offset)); // Define an array of generic vertex attribute data
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
