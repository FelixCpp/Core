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
		void start();

		/* stops the watch */
		void stop();

		/* resets the watch (note: it does not start itself again) */
		void reset();

		/* resets & starts the watch again */
		void restart();

		/* returns true if the stopwatch is currently ticking */
		bool isRunning() const;

		/* returns the elapsed time since last start */
		Duration getElapsedTime() const;

		/* returns a started stopwatch */
		static Stopwatch startNew();

	private:

		/* timepoint of start */
		TimePoint startTime;

		/* timepoint of stop */
		TimePoint endTime;
		
		/* is the watch ticking? */
		bool running;

	};

}