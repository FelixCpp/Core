#pragma once

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)

#include <cstdint>
#include <string>

namespace Core
{

	bool check_al_errors(const std::string & filename, std::uint_fast32_t line);

	template<typename alFunction, typename ... Args>
	auto alCallImpl(const char * filename, const std::uint_fast32_t line, alFunction function, Args ... args) ->
		typename std::enable_if_t<!std::is_same_v<void, decltype(function(args...))>, decltype(function(args...))>
	{
		auto result = function(std::forward<Args>(args)...);
		check_al_errors(filename, line);
		return result;
	}

	template<typename alFunction, typename ... Args>
	auto alCallImpl(const char * filename, const std::uint_fast32_t line, alFunction function, Args ... args) ->
		typename std::enable_if_t<std::is_same_v<void, decltype(function(args...))>, bool>
	{
		function(std::forward<Args>(args)...);
		return check_al_errors(filename, line);
	}

}