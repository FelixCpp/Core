#pragma once

#include <vector>

#include <Core/System/StaticObject.hpp>
#include <Core/System/Datatypes.hpp>

namespace Core
{

	class OpenALSourceIDProvider : public StaticObject {
	public:

		static void generate(i32_t count, u32_t * elements);
		static void clear();

	private:

		static std::vector<u32_t> sources;

	};

	class OpenALBufferIDProvider : public StaticObject {
	public:

		static void generate(i32_t count, u32_t * elements);
		static void clear();

	private:

		static std::vector<u32_t> buffers;
		
	};

}