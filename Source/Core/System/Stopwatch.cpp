// 
// Stopwatch.cpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/System/Stopwatch.hpp>

namespace Core
{
	////////////////////////////////////////////////////////////
	Stopwatch Stopwatch::StartNew()
	{
		Stopwatch watch;
		watch.Start();
		return watch;
	}

	////////////////////////////////////////////////////////////
	Stopwatch::Stopwatch():
		isTicking(false)
	{
	}

	////////////////////////////////////////////////////////////
	void Stopwatch::Start()
	{
		// ignore cases where the stopwatch is already ticking
		if(!isTicking)
		{
			start -= end - Clock::now();
			isTicking = true;
		}
	}

	Time Stopwatch::Stop()
	{
		// ignore cases where the stopwatch isn't ticking
		if(isTicking)
		{
			end = Clock::now();
			isTicking = false;
		}

		return GetElapsedTime();
	}

	Time Stopwatch::GetElapsedTime() const
	{
		// get the current time stamp
		const auto now = isTicking ? Clock::now() : end;

		// compute elapsed time in nanoseconds
		const std::chrono::nanoseconds diff = now - start;

		// create time stamp
		return Nanoseconds(diff.count());
	}

	Time Stopwatch::Restart()
	{
		// reset
		const Time elapsed = Reset();

		// start again
		Start();

		// return elapsed time
		return elapsed;
	}

	Time Stopwatch::Reset()
	{
		// get elapsed time
		const Time elapsed = GetElapsedTime();

		// store timestamp
		start	= Clock::now();
		end		= start;

		// return elapsed time
		return elapsed;
	}

	bool Stopwatch::IsTicking() const
	{
		return isTicking;
	}
}