#pragma once

#include <vector>

#include <Core/System/StaticObject.hpp>
#include <Core/System/Datatypes.hpp>

namespace Core
{

	class OpenALSourceIDProvider : public StaticObject {
	public:

		static void Generate(Int32 count, UInt32 * elements);
		static void Clear();

	private:

		static std::vector<UInt32> sources;

	};

	class OpenALBufferIDProvider : public StaticObject {
	public:

		static void Generate(Int32 count, UInt32 * elements);
		static void Clear();

	private:

		static std::vector<UInt32> buffers;
		
	};

}