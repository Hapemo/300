#ifndef VAO_HPP
#define VAO_HPP

#include "Common.hpp"

namespace CS380
{
	class VAO
	{
	public:
		GLuint GetID() const { return mID; }

		void AddAttribute(GLuint attribIndex, GLuint bindingIndex, GLint size, GLenum type, GLuint internalOffset = 0) const;
		void AddAttributeDivisor(GLuint bindingIndex, GLuint divisor) const;	// divisor = 1 for instancing
		void AttachVerterBuffer(GLuint vboID, GLuint bindingIndex, GLintptr offset, GLsizei stride) const;

		void Create();
		void Destroy();

		void Bind() const;
		void Unbind() const;

	private:
		GLuint mID;
	};
}

#endif