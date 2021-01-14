#pragma once

#include <vector>

#include <Core/System/StaticObject.hpp>
#include <Core/System/Datatypes.hpp>

namespace Core
{

	class OpenALSourceIDProvider : public StaticObject {
	public:

		static void Generate(i32_t count, u32_t * elements);
		static void Clear();

	private:

		static std::vector<u32_t> sources;

	};

	class OpenALBufferIDProvider : public StaticObject {
	public:

		static void Generate(i32_t count, u32_t * elements);
		static void Clear();

	private:

		static std::vector<u32_t> buffers;
		
	};

}