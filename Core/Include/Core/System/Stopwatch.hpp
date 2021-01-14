#pragma once

#include <Core/System/Duration.hpp>

#include <chrono>

namespace Core
{

	class Stopwatch {
	private:

		typedef std::chrono::high_resolution_clock Clock;
		typedef std::chrono::high_resolution_clock::time_point TimePoint;

	public:

		/* default constructor */
		Stopwatch();

		/* starts the watch */
		void Start();

		/* stops the watch */
		void Stop();

		/* resets the watch (note: it does not start itself again) */
		void Reset();

		/* resets & starts the watch again */
		void Restart();

		/* returns true if the stopwatch is currently ticking */
		bool IsRunning() const;

		/* returns the elapsed time since last start */
		Duration GetElapsedTime() const;

		/* returns a started stopwatch */
		static Stopwatch StartNew();

	private:

		/* timepoint of start */
		TimePoint startTime;

		/* timepoint of stop */
		TimePoint endTime;
		
		/* is the watch ticking? */
		bool running;

	};

}