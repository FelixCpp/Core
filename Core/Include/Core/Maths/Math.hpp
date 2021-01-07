#pragma once

#include <Core/System/StaticObject.hpp>
#include <Core/System/Datatypes.hpp>

#include <type_traits>
#include <cmath>

namespace Core
{
	
	template<typename T>
	struct MathImpl : StaticObject {

		// Fundamental constants
		inline static constexpr T Pi				= (T)3.141592653589793238462643383279502884;
		inline static constexpr T E					= (T)2.71828182845904523536028747135266249;
		inline static constexpr T EulersConstant	= (T)0.5772156649015328606065120900824;
		inline static constexpr T GoldenRatio		= (T)1.61803398874989484820458683436;
		
		// Expressions involving fundamental constants
		inline static constexpr T TwoPi				= (T)6.283185307179586476925286766558;
		inline static constexpr T PiOverTwo			= (T)1.570796326794896619231321691639751;
		inline static constexpr T PiOverFour		= (T)0.785398163397448309615660845819876;
		inline static constexpr T LogTwoPi			= (T)1.837877066409345483560659472811235279723;
		inline static constexpr T PiSquared			= (T)9.869604401089358618834490999873;
		inline static constexpr T SquaredPi			= (T)1.772453850905516027298167483341145;
		inline static constexpr T SqrtTwoPi			= (T)2.506628274631000502415765284811045;
		inline static constexpr T OneOverSqrtPi		= (T)0.56418958354775628694807945156082;
		inline static constexpr T OneOverSqrtTwoPi	= (T)0.39894228040143267793994605993439;

		// Square roots of small integers.
		inline static constexpr T Sqrt2				= (T)1.414213562373095048801688724209698;
		inline static constexpr T Sqrt3				= (T)1.732050807568877293527446341505872;
		inline static constexpr T Sqrt5				= (T)2.236067977499789696409173668731276;
		inline static constexpr T Sqrt7				= (T)2.645751311064590590501615753639260;
		inline static constexpr T Sqrt17			= (T)4.1231056256176605498214098559741;

		// Logarithms of small integers.
		inline static constexpr T Log2				= (T)0.6931471805599453094172321214581765681;
		inline static constexpr T Log3				= (T)1.0986122886681096913952452369225;
		inline static constexpr T Log10				= (T)2.302585092994045684017991454684364;
		inline static constexpr T Log17				= (T)2.8332133440562160802495346178731;
		inline static constexpr T InvLog10			= (T)0.434294481903251827651128918916605;

		inline static T lerp(T v0, T v1, T t)
		{
			return ((1 - t) * v0) + (t * v1);
		}

		inline static T lerp(T v0, T v1, T t, T execute)
		{
			const T value = MathImpl::lerp(v0, v1, t);
			const T diff = (value - v1);
			
			if (diff <= execute)
			{
				return v1;
			}

			return value;
		}

		inline static T distance(T x1, T y1, T x2, T y2)
		{
			const T xDiff = x2 - x1;
			const T yDiff = y2 - y1;
			const T lengthSq = (xDiff * xDiff) + (yDiff * yDiff);
			return std::sqrt(lengthSq);
		}

		inline static T constrain(T value, T min, T max)
		{
			return value <= min ? min : value >= max ? max : value;
		}

		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		inline static T map(T value, T istart, T istop, T ostart, T ostop)
		{
			return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
		}

		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		inline static T cmap(T value, T istart, T istop, T ostart, T ostop)
		{
			const T output = MathImpl::map(value, istart, istop, ostart, ostop);
			const T min = std::fmin(ostart, ostop);
			const T max = std::fmax(ostart, ostop);

			return constrain(output, min, max);
		}

		inline static T degrees(T radians)
		{
			return radians * (T)180 / MathImpl::Pi;
		}

		inline static T radians(T degrees)
		{
			return degrees * MathImpl::Pi / (T)180;
		}

	};

	typedef MathImpl<float> FMath;
	typedef MathImpl<double> Math;

}