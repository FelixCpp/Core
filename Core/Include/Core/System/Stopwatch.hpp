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

		Stopwatch();

		void start();
		void stop();
		void reset();
		void restart();

		bool isRunning() const;
		Duration getElapsedTime() const;

		static Stopwatch startNew();

	private:

		TimePoint startTime;
		TimePoint endTime;
		
		bool running;

	};

}