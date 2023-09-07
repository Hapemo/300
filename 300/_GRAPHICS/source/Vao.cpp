#include "Vao.hpp"

void GFX::VAO::AddAttribute(GLuint attribIndex, GLuint bindingIndex, GLint size, GLenum type, GLuint internalOffset) const
{
	glEnableVertexArrayAttrib(mID, attribIndex);										// Enable attribute index (location in shader)
	glVertexArrayAttribFormat(mID, attribIndex, size, type, GL_FALSE, internalOffset);	// Internal Format of attribute in VBO, attrib index
	glVertexArrayAttribBinding(mID, attribIndex, bindingIndex);							// Attrib index, binding index to vao
}

void GFX::VAO::AddAttributeDivisor(GLuint bindingIndex, GLuint divisor) const
{
	glVertexArrayBindingDivisor(mID, bindingIndex, divisor);	// Pull next data after every instance instead of every vertex
}

void GFX::VAO::AttachVerterBuffer(GLuint vboID, GLuint bindingIndex, GLintptr offset, GLsizei stride) const
{
	glVertexArrayVertexBuffer(mID, bindingIndex, vboID, offset, stride);	// Binding Index in VAO
}

void GFX::VAO::Create()
{
	glCreateVertexArrays(1, &mID);
}

void GFX::VAO::Destroy()
{
	glDeleteVertexArrays(1, &mID);
}

void GFX::VAO::Bind() const
{
	glBindVertexArray(mID);
}

void GFX::VAO::Unbind() const
{
	glBindVertexArray(0);
}
