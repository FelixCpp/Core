#pragma once

#include <functional>

namespace Core
{

	template<typename T>
	struct Function; // forward declaration

	template<typename TReturnType, typename ... TArgs>
	struct Function<TReturnType(TArgs ...)> : std::function<TReturnType(TArgs...)> {
		using std::function<TReturnType(TArgs...)>::function; // use the base-constructor
	};

	// handy definitions
	template<typename ... TArgs> using Action = Function<void(TArgs ...)>;
	template<typename ... TArgs> using Predicate = Function<bool(TArgs ...)>;

}