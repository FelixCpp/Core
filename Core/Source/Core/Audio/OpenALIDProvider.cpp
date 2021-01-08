#include <Core/Audio/OpenALIDProvider.hpp>

#include <al.h>

namespace Core
{

	std::vector<u32_t> OpenALSourceIDProvider::sources;
	std::vector<u32_t> OpenALBufferIDProvider::buffers;

	void OpenALSourceIDProvider::generate(i32_t count, u32_t * elements)
	{
		alGenSources(count, elements);

		for (i32_t i = 0; i < count; i++)
			sources.push_back(elements[i]);
	}

	void OpenALSourceIDProvider::clear()
	{
		alDeleteSources(sources.size(), sources.data());
	}

	void OpenALBufferIDProvider::generate(i32_t count, u32_t * elements)
	{
		alGenBuffers(count, elements);
		
		for (i32_t i = 0; i < count; i++)
			buffers.push_back(elements[i]);
	}

	void OpenALBufferIDProvider::clear()
	{
		alDeleteBuffers(buffers.size(), buffers.data());
	}

}