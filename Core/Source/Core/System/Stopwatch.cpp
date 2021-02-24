#include <Core/System/Stopwatch.hpp>

namespace Core
{

	Stopwatch::Stopwatch() :
		running(false),
		startTime(Clock::now()),
		endTime(Clock::now())
	{
	}

	void Stopwatch::Start()
	{
		if (!this->running)
		{
			this->running = true;
			this->startTime -= this->endTime - Clock::now();
		}
	}

	void Stopwatch::Stop()
	{
		if (this->running)
		{
			this->running = false;
			this->endTime = Clock::now();
		}
	}

	void Stopwatch::Reset()
	{
		this->running = false;
		this->startTime = Clock::now();
		this->endTime = Clock::now();
	}

	void Stopwatch::Restart()
	{
		this->Reset();
		this->Start();
	}

	bool Stopwatch::IsRunning() const
	{
		return this->running;
	}

	TimeSpan Stopwatch::GetElapsedTime() const
	{
		const TimePoint tp = this->running ? Clock::now() : this->endTime;
		const std::chrono::microseconds time = std::chrono::duration_cast<std::chrono::microseconds>(tp - this->startTime);
		return TimeSpan::FromMicroseconds(time.count());
	}

	Stopwatch Stopwatch::StartNew()
	{
		Stopwatch watch;
		watch.Start();
		return watch;
	}

}