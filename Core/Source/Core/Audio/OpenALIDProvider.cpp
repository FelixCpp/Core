#include <Core/Audio/OpenALIDProvider.hpp>

#include <al.h>

namespace Core
{

	std::vector<UInt32> OpenALSourceIDProvider::sources;
	std::vector<UInt32> OpenALBufferIDProvider::buffers;

	void OpenALSourceIDProvider::Generate(Int32 count, UInt32 * elements)
	{
		alGenSources(count, elements);

		for (Int32 i = 0; i < count; i++)
			sources.push_back(elements[i]);
	}

	void OpenALSourceIDProvider::Clear()
	{
		alDeleteSources(sources.size(), sources.data());
	}

	void OpenALBufferIDProvider::Generate(Int32 count, UInt32 * elements)
	{
		alGenBuffers(count, elements);
		
		for (Int32 i = 0; i < count; i++)
			buffers.push_back(elements[i]);
	}

	void OpenALBufferIDProvider::Clear()
	{
		alDeleteBuffers(buffers.size(), buffers.data());
	}

}