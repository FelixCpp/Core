#pragma once

#include <memory>

#include <Core/System/DelegateBase.hpp>
#include <Core/System/Collection.hpp>

namespace Core
{

	template<typename>
	class Event;

	template<typename TReturnType, typename ... TArgs>
	class Event<TReturnType(TArgs ...)> {
	public:

		using DelegateType = DelegateBase<TReturnType(TArgs ...)>;
		using EventItem = std::shared_ptr<DelegateType>;

	public:

		Event() :
			delegates()
		{
		}

		inline Event & operator+=(const EventItem & delegate)
		{
			if (delegate != nullptr)
			{
				if (*delegate != nullptr)
				{
					this->delegates.emplace_back(delegate);
				}
			}

			return *this;
		}

		inline Event & operator-=(const EventItem & delegate)
		{
			const auto itr = std::find_if(this->delegates.begin(), this->delegates.end(), [&delegate](const EventItem & item) -> bool
			{
				return item->Equals(*delegate);
			});

			if (itr != this->delegates.end())
			{
				this->delegates.erase(itr);
			}

			return *this;
		}

		inline Event & RemoveAll(const EventItem & item)
		{
			this->delegates.remove_if([&item](const EventItem & current) -> bool
			{
				return current->equals(*item);
			});

			return *this;
		}

		inline TReturnType Invoke(const TArgs & ... arguments) const
		{
			if constexpr (std::is_void_v<TReturnType>)
			{
				for (const EventItem & item : this->delegates)
				{
					const DelegateType & delegate = *item;
					delegate.invoke(std::forward<decltype(arguments)>(arguments)...);
				}
			} else
			{
				TReturnType result = this->delegates->front().invoke(std::forward<decltype(arguments)>(arguments)...);

				for (size_t i = 1; i < this->delegates.size(); i++)
				{
					const DelegateType & delegate = *this->delegates[i];
					if (delegate != nullptr)
					{
						result = delegate.invoke(std::forward<decltype(arguments)>(arguments)...);
					}
				}

				return result;
			}
		}

		inline TReturnType operator()(const TArgs & ... arguments) const
		{
			return this->invoke(std::forward<decltype(arguments)>(arguments)...);
		}

		const List<EventItem> & GetDelegates() const
		{
			return this->delegates;
		}

	private:

		List<EventItem> delegates;

	};

	template<class TSender, typename ... TArgs>
	using EventHandler = Event<void(TSender *, TArgs ...)>;

}