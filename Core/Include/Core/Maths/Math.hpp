#pragma once

#include <Core/System/StaticObject.hpp>
#include <Core/System/Datatypes.hpp>

#include <type_traits>
#include <cmath>
#include <iostream>

namespace Core
{
	
	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	struct Math : StaticObject {

		inline static T minOf(T a, T b)
		{
			return a < b ? a : b;
		}

		inline static T maxOf(T a, T b)
		{
			return a > b ? a : b;
		}

		inline static T lerp(T v0, T v1, T t)
		{
			return (1 - t) * v0 + t * v1;
		}

		inline static T lerp(T v0, T v1, T t, T execute)
		{
			T value = lerp(v0, v1, t);
			
			if (std::abs(value - v1) <= execute)
			{
				return v1;
			}

			return value;
		}

		inline static T constrain(T value, T min, T max)
		{
			return value < min ? min : value > max ? max : value;
		}

		inline static T map(T value, T istart, T istop, T ostart, T ostop)
		{
			return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
		}

		inline static T cmap(T value, T istart, T istop, T ostart, T ostop)
		{
			const T output = ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
			const T start = minOf(ostart, ostop);
			const T stop = maxOf(ostart, ostop);
			return constrain(output, start, stop);
		}

	};

	typedef Math<float> FMath;
	typedef Math<double> DMath;
	typedef Math<i32_t> IMath;
	typedef Math<u32_t> UMath;

}