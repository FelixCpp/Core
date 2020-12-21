#include <Core/System/Timer.hpp>

#include <thread>

namespace Core
{

	Timer::Timer() :
		active(false)
	{
	}
	
	void Timer::setTimeout(const CompletionCallback & completion, const Duration & delay)
	{
		this->active = true;

		std::thread workerThread([=]()
		{
			if (!this->active) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay.toMilliseconds()));
			if (!this->active) return;
			completion();
		});

		workerThread.detach();
	}
	
	void Timer::setInterval(const CompletionCallback & completion, const Duration & interval)
	{
		this->active = true;

		std::thread workerThread([=]()
		{
			while (true)
			{
				if (!this->active) return;
				std::this_thread::sleep_for(std::chrono::milliseconds(interval.toMilliseconds()));
				if (!this->active) return;

				completion();
			}
		});

		workerThread.detach();
	}
	
	void Timer::stop()
	{
		this->active = false;
	}

}