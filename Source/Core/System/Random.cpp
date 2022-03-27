// 
// Random.cpp
// Core
// 
// Created by Felix Busch on 24.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/System/Random.hpp>

namespace Core
{

	////////////////////////////////////////////////////////////
	/// Link static members
	/// 
	////////////////////////////////////////////////////////////
	Random::RandomDevice Random::device;
	Random::DefaultRandomEngine Random::engine(device());

	////////////////////////////////////////////////////////////
	void Random::SetSeed(SeedType seed)
	{
		engine.seed(seed);
	}


}