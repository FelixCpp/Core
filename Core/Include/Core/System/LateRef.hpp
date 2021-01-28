#pragma once

/// <summary>
/// C++ / STL
/// </summary>
#include <type_traits>

namespace Core
{

	template<class T>
	using LateRef = std::remove_pointer_t<std::remove_reference_t<T>> *&;

	//template<class T>
	//class LateRef {
	//public:
	//
	//	using raw_type = std::remove_cv_t<T>;
	//	using value_type = raw_type *&;
	//
	//	LateRef(value_type value) :
	//		value(value)
	//	{ }
	//
	//private:
	//
	//	value_type value;
	//
	//};

}