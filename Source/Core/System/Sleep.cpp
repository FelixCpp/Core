// 
// Sleep.cpp
// Core
// 
// Created by Felix Busch on 07.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/System/Sleep.hpp>
#include <Core/System/Types.hpp>

#include <Windows.h>
#pragma comment(lib, "Winmm")

namespace Core
{

	void Sleep::Perform(const Time& duration)
	{
		// extract the number of milliseconds from the duration
		const auto dwMilliseconds = duration.ToMilliseconds<i64>();

		// make sure to avoid sleeping for 0 milliseconds or even negative time
		if(dwMilliseconds > 0)
		{
            // Get the minimum supported timer resolution on this system
            static const UINT periodMin = []
            {
                TIMECAPS tc;
                timeGetDevCaps(&tc, sizeof(TIMECAPS));
                return tc.wPeriodMin;
            }();

            // Set the timer resolution to the minimum for the Sleep call
            timeBeginPeriod(periodMin);

            // Wait...
            ::Sleep((DWORD)dwMilliseconds);

            // Reset the timer resolution back to the system default
            timeEndPeriod(periodMin);
		}
	}

}