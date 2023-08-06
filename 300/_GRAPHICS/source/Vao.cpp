#include "Vao.hpp"

void CS380::VAO::AddAttribute(GLuint attribIndex, GLuint bindingIndex, GLint size, GLenum type, GLuint internalOffset) const
{
	glEnableVertexArrayAttrib(mID, attribIndex);										// Enable attribute index (location in shader)
	glVertexArrayAttribFormat(mID, attribIndex, size, type, GL_FALSE, internalOffset);	// Internal Format of attribute in VBO, attrib index
	glVertexArrayAttribBinding(mID, attribIndex, bindingIndex);							// Attrib index, binding index to vao
}

void CS380::VAO::AddAttributeDivisor(GLuint bindingIndex, GLuint divisor) const
{
	glVertexArrayBindingDivisor(mID, bindingIndex, divisor);	// Pull next data after every instance instead of every vertex
}

void CS380::VAO::AttachVerterBuffer(GLuint vboID, GLuint bindingIndex, GLintptr offset, GLsizei stride) const
{
	glVertexArrayVertexBuffer(mID, bindingIndex, vboID, offset, stride);	// Binding Index in VAO
}

void CS380::VAO::Create()
{
	glCreateVertexArrays(1, &mID);
}

void CS380::VAO::Destroy()
{
	glDeleteVertexArrays(1, &mID);
}

void CS380::VAO::Bind() const
{
	glBindVertexArray(mID);
}

void CS380::VAO::Unbind() const
{
	glBindVertexArray(0);
}
