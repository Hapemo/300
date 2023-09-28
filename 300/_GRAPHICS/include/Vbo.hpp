/**-------------------------------------------------------------------------------------/
 * @file
 *  VBO.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622, f.lee@digipen.edu
 * @date
 *  2023/05/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef VBO_HPP
#define VBO_HPP

#include "Common.hpp"

namespace GFX
{
	class VBO
	{
	public:
		GLuint GetID() const { return mID; }

		void Create(GLsizeiptr bufferSize);

		void AttachData(GLintptr offset, GLsizeiptr dataSize, const void* data) const;

		void Destroy();

	private:
		GLuint mID = 0;
	};
}

#endif