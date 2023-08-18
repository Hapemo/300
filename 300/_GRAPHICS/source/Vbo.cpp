#include "Vbo.hpp"

void GFX::VBO::Create(GLsizeiptr bufferSize)
{
	glCreateBuffers(1, &mID);
	glNamedBufferStorage(mID, bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void GFX::VBO::AttachData(GLintptr offset, GLsizeiptr dataSize, const void* data) const
{
	glNamedBufferSubData(mID, offset, dataSize, data);
}

void GFX::VBO::Destroy()
{
	glDeleteBuffers(1, &mID);
}
