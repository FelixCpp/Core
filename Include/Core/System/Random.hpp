// 
// Random.hpp
// Core
// 
// Created by Felix Busch on 24.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <random>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define random number generator implemented using
	///		   the STL random library
	/// 
	////////////////////////////////////////////////////////////
	class Random
	{
	public:

		////////////////////////////////////////////////////////////
		/// Types
		/// 
		////////////////////////////////////////////////////////////
		using DefaultRandomEngine	= std::default_random_engine;
		using RandomDevice			= std::random_device;
		using SeedType				= std::default_random_engine::result_type;

	public:

		////////////////////////////////////////////////////////////
		/// Type ensurance
		/// 
		////////////////////////////////////////////////////////////
		template<typename T> inline static constexpr bool IsIntegral = std::_Is_any_of_v<T, short, int, long, long long, unsigned short, unsigned int, unsigned long, unsigned long long>;
		template<typename T> inline static constexpr bool IsFloatingPoint = std::_Is_any_of_v<T, float, double, long double>;
		template<typename T> inline static constexpr bool IsGeneratable = IsIntegral<T> || IsFloatingPoint<T>;

	public:

		////////////////////////////////////////////////////////////
		/// \brief Get a random number between \a min and \a max
		///
		///	\param min The lower bound of the generated number
		///	\param max The upper bound of the generated number
		/// 
		////////////////////////////////////////////////////////////
		template<typename T> requires IsGeneratable<T>
		static T Get(T min, T max)
		{
			// generate integral number
			if constexpr (IsIntegral<T>)
			{
				std::uniform_int_distribution<T> distribution(min, max);
				return distribution(engine);
			}

			// generate floating point number
			if constexpr (IsFloatingPoint<T>)
			{
				std::uniform_real_distribution<T> distribution(min, max);
				return distribution(engine);
			}

			// return default
			return T{};
		}

		////////////////////////////////////////////////////////////
		/// \brief Get a random number between 0 and \a max
		///
		///	\param max The upper bound of the generated number. This value must be above 0
		/// 
		////////////////////////////////////////////////////////////
		template<typename T> requires IsGeneratable<T>
		static T Get(T max)
		{
			return Get((T)0, max);
		}

		////////////////////////////////////////////////////////////
		/// \brief Set the seed to generate the random numbers on
		///
		///	\param seed The new seed to use when generating random numbers
		/// 
		////////////////////////////////////////////////////////////
		static void SetSeed(SeedType seed);

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		static RandomDevice device;			//!< The device to generate on
		static DefaultRandomEngine engine;	//!< The engine to generate with

	};
}