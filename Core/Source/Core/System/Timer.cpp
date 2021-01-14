#include <Core/System/Timer.hpp>

#include <thread>

namespace Core
{

	Timer::Timer() :
		active(false)
	{
	}
	
	void Timer::SetTimeout(const CompletionCallback & completion, const Duration & delay)
	{
		this->active = true;

		std::thread workerThread([=]()
		{
			if (!this->active) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay.ToMilliseconds()));
			if (!this->active) return;
			completion();
		});

		workerThread.detach();
	}
	
	void Timer::SetInterval(const CompletionCallback & completion, const Duration & interval)
	{
		this->active = true;

		std::thread workerThread([=]()
		{
			while (true)
			{
				if (!this->active) return;
				std::this_thread::sleep_for(std::chrono::milliseconds(interval.ToMilliseconds()));
				if (!this->active) return;

				completion();
			}
		});

		workerThread.detach();
	}
	
	void Timer::Stop()
	{
		this->active = false;
	}

}