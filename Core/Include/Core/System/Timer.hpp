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

		void setTimeout(const CompletionCallback & completion, const Duration & delay);
		void setInterval(const CompletionCallback & completion, const Duration & interval);
		void stop();

	private:

		bool active;

	};

}