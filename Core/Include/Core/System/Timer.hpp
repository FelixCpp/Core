#pragma once

#include <Core/System/Function.hpp>
#include <Core/System/Datatypes.hpp>

#include <Core/System/TimeSpan.hpp>

namespace Core
{

	class Timer {
	public:

		typedef Action<> CompletionCallback;

	public:

		Timer();

		void SetTimeout(const CompletionCallback & completion, const TimeSpan & delay);
		void SetInterval(const CompletionCallback & completion, const TimeSpan & interval);
		void Stop();

	private:

		bool active;

	};

}