#include "Vao.hpp"


void GFX::VAO::AddAttribute(GLuint attribIndex, GLuint bindingIndex, GLint size, GLenum type, GLuint internalOffset) const
{
	// Specifies which attribute index will be used (location in shader)
	glEnableVertexArrayAttrib(mID, attribIndex);						

	// How to interpret the format (type, size, offset) of this particular attribute in VBO
	glVertexArrayAttribFormat(mID, attribIndex, size, type, GL_FALSE, internalOffset);

	// Attrib index, binding index to vao
	glVertexArrayAttribBinding(mID, attribIndex, bindingIndex);							
}


void GFX::VAO::AddAttributeDivisor(GLuint bindingIndex, GLuint divisor) const
{
	// Specifies the divisor for vertex attribute bindings within the specified VAO.
	// - bindingIndex:	Specifies the index of the vertex attribute binding whose divisor to modify.
	// - divisor:		Specifies how often the vertex attribute data should be advanced when rendering instances.
	// Divisor = 0:			Advance the vertex attribute pointer by one after each vertex. just like non instanced rendering.
	// Divisor = 1:			Advanced after each instance is rendered. Useful when each instances contain the same data
	// Divisor = 2:			Advanced after 2 instances are rendered. When the data is repeated after every 2 instances.

	glVertexArrayBindingDivisor(mID, bindingIndex, divisor);	// Pull next data after every instance instead of every vertex
}


void GFX::VAO::AttachVertexBuffer(GLuint vboID, GLuint bindingIndex, GLintptr offset, GLsizei stride) const
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
