/*!*****************************************************************************
\file Vbo.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Class for the vertex buffer object of a mesh. Provides abstraction of VBO
operations when creating a new mesh
*******************************************************************************/

#ifndef VBO_HPP
#define VBO_HPP

#include "Common.hpp"

namespace GFX
{
	class VBO
	{
	public:

/*!*****************************************************************************
Returns the ID of the VBO
*******************************************************************************/		
		GLuint GetID() const { return mID; }

/*!*****************************************************************************
Creates the VBO with the specified buffer size
*******************************************************************************/		
		void Create(GLsizeiptr bufferSize);

/*!*****************************************************************************
Attach the data specified onto the VBO
*******************************************************************************/		
		void AttachData(GLintptr offset, GLsizeiptr dataSize, const void* data) const;

/*!*****************************************************************************
Destroys the VBO that have been allocated on the GPU memory
*******************************************************************************/		
		void Destroy();

	private:
		GLuint mID = 0;
	};
}

#endif