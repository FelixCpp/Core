#include <Core/Maths/Random.hpp>

namespace Core
{

	const UInt32 Random::DefaultSeed = std::default_random_engine::default_seed;

	std::random_device Random::device;
	std::default_random_engine Random::generator(device());

	void Random::SetSeed(UInt32 seed)
	{
		generator.seed(seed);
	}

}