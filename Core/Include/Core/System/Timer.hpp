#pragma once

#include <Core/System/Function.hpp>
#include <Core/System/Datatypes.hpp>

#include <Core/System/Duration.hpp>

namespace Core
{

	class Timer {
	public:

		typedef Action<> CompletionCallback;

	public:

		Timer();

		void SetTimeout(const CompletionCallback & completion, const Duration & delay);
		void SetInterval(const CompletionCallback & completion, const Duration & interval);
		void Stop();

	private:

		bool active;

	};

}