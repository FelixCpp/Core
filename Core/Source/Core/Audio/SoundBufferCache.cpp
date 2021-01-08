#include <Core/Audio/SoundBufferCache.hpp>

namespace Core
{

	SoundBufferCache::SoundBufferCache() :
		buffers()
	{

	}

	bool SoundBufferCache::contains(const std::string & filepath)
	{
		return this->buffers.find(filepath) != this->buffers.end();
	}

	void SoundBufferCache::set(const std::string & filepath, u32_t bufferID)
	{
		this->buffers[filepath] = bufferID;
	}

	u32_t & SoundBufferCache::get(const std::string & filepath)
	{
		return this->buffers.at(filepath);
	}

}