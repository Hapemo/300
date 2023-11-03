#include "Ssbo.hpp"

void GFX::SSBO::Create(size_t size, int bindLocation)
{
	glCreateBuffers(1, &mID);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mID);
	glNamedBufferData(mID, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindLocation, mID);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GFX::SSBO::SubData(size_t dataSize, const void* data)
{
	glNamedBufferSubData(mID, 0, dataSize, data);
}

GFX::SSBO::~SSBO()
{
	glDeleteBuffers(1, &mID);
}