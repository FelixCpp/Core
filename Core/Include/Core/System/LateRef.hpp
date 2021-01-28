#pragma once

/// <summary>
/// C++ / STL
/// </summary>
#include <type_traits>

namespace Core
{

	template<class T>
	using LateRef = std::remove_pointer_t<std::remove_reference_t<T>> *&;

}