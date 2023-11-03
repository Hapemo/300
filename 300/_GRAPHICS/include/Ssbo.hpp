/*!*****************************************************************************
\file			Ssbo.hpp
\author			Lee Fu Sheng Roy
\par DP email:	f.lee@digipen.edu
\date			3/11/23
\brief
	The declarations for the Shader Storage Buffer Object class

*******************************************************************************/

#ifndef SSBO_HPP

#include "Common.hpp"

namespace GFX
{
	class SSBO
	{
	public:
/***************************************************************************/
/*!
\brief
	Creates the buffer by allocating memory in the GPU. Specifies the 
	binding locations of the SSBO as well
*/
/**************************************************************************/
		void Create(size_t size, int bindLocation);

/***************************************************************************/
/*!
\brief
	Copies data provided into the buffer on the GPU storage
*/
/**************************************************************************/
		void SubData(size_t dataSize, const void* data);

		~SSBO();

	private:
		unsigned mID;
	};
}

#endif // !SSBO_HPP

