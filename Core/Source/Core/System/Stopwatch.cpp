#include <Core/System/Stopwatch.hpp>

namespace Core
{

	Stopwatch::Stopwatch() :
		running(false),
		startTime(Clock::now()),
		endTime(Clock::now())
	{
	}

	void Stopwatch::start()
	{
		if (!this->running)
		{
			this->running = true;
			this->startTime -= this->endTime - Clock::now();
		}
	}

	void Stopwatch::stop()
	{
		if (this->running)
		{
			this->running = false;
			this->endTime = Clock::now();
		}
	}

	void Stopwatch::reset()
	{
		this->running = false;
		this->startTime = Clock::now();
		this->endTime = Clock::now();
	}

	void Stopwatch::restart()
	{
		this->reset();
		this->start();
	}

	bool Stopwatch::isRunning() const
	{
		return this->running;
	}

	Duration Stopwatch::getElapsedTime() const
	{
		const TimePoint tp = this->running ? Clock::now() : this->endTime;
		const std::chrono::microseconds time = std::chrono::duration_cast<std::chrono::microseconds>(tp - this->startTime);
		return Duration::fromMicroseconds(time.count());
	}

	Stopwatch Stopwatch::startNew()
	{
		Stopwatch watch;
		watch.start();
		return watch;
	}

}