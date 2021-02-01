#pragma once

#include <iostream>
#include <memory>

namespace Core
{

	template<typename ReturnValue, typename ... Args>
	class Callable;

	template<typename ReturnType, typename ... Args>
	class Callable<ReturnType(Args ...)> {
	public:

		class IExecutable {
		public:

			virtual ~IExecutable() = default;
			virtual ReturnType Invoke(Args && ... arguments) const = 0;
		
		};

		template<class Lambda>
		class Executable : public IExecutable {
		public:

			explicit Executable(const Lambda & lambda) :
				lambda(lambda)
			{ }

			virtual ReturnType Invoke(Args && ... arguments) const override
			{
				return lambda(std::forward<Args &&>(arguments)...);
			}

		private:

			Lambda lambda;

		};

	public:

		Callable() :
			executable(nullptr)
		{ }

		template<class Lambda, typename = std::enable_if_t<
			std::is_invocable_v<Lambda, Args ...> &&
			std::is_same_v<std::invoke_result_t<Lambda, Args ...>, ReturnType>	
		>>
		Callable(const Lambda & lambda) :
			executable(new Executable<Lambda>(lambda))
		{
		}

		operator bool() const
		{
			return (bool)executable;
		}

		ReturnType operator()(Args && ... arguments) const
		{
			return executable->Invoke(std::forward<Args &&>(arguments)...);
		}

	private:

		std::shared_ptr<IExecutable> executable;

	};

}