#include "Vbo.hpp"

void CS380::VBO::Create(GLsizeiptr bufferSize)
{
	glCreateBuffers(1, &mID);
	glNamedBufferStorage(mID, bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void CS380::VBO::AttachData(GLintptr offset, GLsizeiptr dataSize, const void* data) const
{
	glNamedBufferSubData(mID, offset, dataSize, data);
}

void CS380::VBO::Destroy()
{
	glDeleteBuffers(1, &mID);
}
