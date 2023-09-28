/*!*****************************************************************************
\file Vao.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Class for the vertex array object of a mesh. Provides abstraction of VAO
operations when creating a new mesh
*******************************************************************************/

#ifndef VAO_HPP
#define VAO_HPP

#include "Common.hpp"

namespace GFX
{
	class VAO
	{
	public:
/*!*****************************************************************************
Returns the ID of the VAO
*******************************************************************************/
		GLuint GetID() const { return mID; }

/*!*****************************************************************************
Enables the vertex array attribute on the VAO at the given attribute index
and binding index
*******************************************************************************/
		void AddAttribute(GLuint attribIndex, GLuint bindingIndex, GLint size, GLenum type, GLuint internalOffset = 0) const;

/*!*****************************************************************************
Sets the divisor of the binding index for instanced rendering
*******************************************************************************/
		void AddAttributeDivisor(GLuint bindingIndex, GLuint divisor) const;

/*!*****************************************************************************
Attaches the vertex buffer object to the VAO's binding point
*******************************************************************************/
		void AttachVertexBuffer(GLuint vboID, GLuint bindingIndex, GLintptr offset, GLsizei stride) const;

/*!*****************************************************************************
Creates the vertex array object
*******************************************************************************/
		void Create();

/*!*****************************************************************************
Destroys the VAO allocated on the GPU memory
*******************************************************************************/
		void Destroy();

/*!*****************************************************************************
Binds the VAO to the openGL context
*******************************************************************************/
		void Bind() const;

/*!*****************************************************************************
Unbinds the VAO from the openGL context
*******************************************************************************/
		void Unbind() const;

	private:
		GLuint mID;
	};
}

#endif