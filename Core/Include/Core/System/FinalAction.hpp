#pragma once

#include <type_traits>

namespace Core
{

	template<typename TFunction, typename = std::enable_if_t<
		std::is_invocable_v<TFunction> &&
		std::is_same_v<std::result_of_t<TFunction()>, void>
	>>
	class FinalAction {
	public:

		inline FinalAction(TFunction function) :
			function(function),
			invoke(true)
		{ }

		inline FinalAction(FinalAction && other) :
			function(std::move(other.function)),
			invoke(other.invoke)
		{
			other.invoke = false;
		}

		inline FinalAction(const FinalAction &) = delete;
		inline FinalAction & operator=(const FinalAction &) = delete;

		inline ~FinalAction()
		{
			if (this->invoke)
				this->function();
		}

	private:

		TFunction function;
		bool invoke;

	};

}