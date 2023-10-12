#ifndef SSBO_HPP

#include "Common.hpp"

namespace GFX
{
	class SSBO
	{
	public:
		void Create(size_t size, int bindLocation);

		void SubData(size_t dataSize, const void* data);

	private:
		unsigned mID;
	};
}

#endif // !SSBO_HPP

