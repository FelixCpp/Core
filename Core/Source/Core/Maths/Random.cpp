#include <Core/Maths/Random.hpp>

namespace Core
{

	const u32_t Random::DefaultSeed = std::default_random_engine::default_seed;

	std::random_device Random::device;
	std::default_random_engine Random::generator(device());

	void Random::SetSeed(u32_t seed)
	{
		generator.seed(seed);
	}

}