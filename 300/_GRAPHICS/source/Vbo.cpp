#include "Vbo.hpp"

void GFX::VBO::Create(GLsizeiptr bufferSize)
{
	// Create one buffer object within the server, and get the handle ID
	glCreateBuffers(1, &mID);

	// Allocate memory for the buffer object
	glNamedBufferStorage(mID, bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void GFX::VBO::AttachData(GLintptr offset, GLsizeiptr dataSize, const void* data) const
{
	// Used to copy data into the VBO. but only replaces a range of data into the existing buffer, starting
	// from the offset.
	glNamedBufferSubData(mID, offset, dataSize, data);
}

void GFX::VBO::Destroy()
{
	// After the VBO is deleted, its contents will be lost
	glDeleteBuffers(1, &mID);
}
