#pragma once

#include <string>
#include <unordered_map>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	class SoundBufferCache {
	public:

		/*
			NOTE:
				This cache does not manage the allocation &
				deallocation of any SoundBuffer
		*/

		/* the default constructor */
		SoundBufferCache();

		/* returns true if the cache contains a value behind the given key */
		bool contains(const std::string & filepath);

		/* sets the value behind the key */
		void set(const std::string & filepath, u32_t bufferID);
		
		/* returns the value behind the key */
		u32_t & get(const std::string & filepath);

	private:

		/* unordered_map as datastructure for caching */
		std::unordered_map<std::string, u32_t> buffers;

	};

}