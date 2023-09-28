/**-------------------------------------------------------------------------------------/
 * @file
 *  VAO.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef VAO_HPP
#define VAO_HPP

#include "Common.hpp"

namespace GFX
{
	class VAO
	{
	public:
		GLuint GetID() const { return mID; }

		void AddAttribute(GLuint attribIndex, GLuint bindingIndex, GLint size, GLenum type, GLuint internalOffset = 0) const;
		void AddAttributeDivisor(GLuint bindingIndex, GLuint divisor) const;	// divisor = 1 for instancing
		void AttachVertexBuffer(GLuint vboID, GLuint bindingIndex, GLintptr offset, GLsizei stride) const;

		void Create();
		void Destroy();

		void Bind() const;
		void Unbind() const;

	private:
		GLuint mID;
	};
}

#endif