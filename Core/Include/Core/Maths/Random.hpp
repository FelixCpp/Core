#pragma once

#include <random>
#include <Core/System/Datatypes.hpp>
#include <Core/System/StaticObject.hpp>

namespace Core
{

	class Random : public StaticObject {
	private:

		template<typename T> inline static constexpr bool IsInt  = std::_Is_any_of_v<T, short, int, long, long long, unsigned short, unsigned int, unsigned long, unsigned long long>;
		template<typename T> inline static constexpr bool IsReal = std::_Is_any_of_v<T, float, double, long double>;
		template<typename T> inline static constexpr bool IsValid = IsInt<T> || IsReal<T>;

	public:

		static const u32_t DefaultSeed;

		template<typename T, typename = std::enable_if_t<IsValid<T>>>
		static T get(T min, T max)
		{
			if constexpr (IsInt<T>)
			{
				std::uniform_int_distribution<T> distribution(min, max);
				return distribution(generator);
			} else if constexpr (IsReal<T>)
			{
				std::uniform_real_distribution<T> distribution(min, max);
				return distribution(generator);
			}

			return T();
		}

		template<typename T, typename = std::enable_if_t<IsValid<T>>>
		static T get(T max)
		{
			return get((T)0, max);
		}

		static void setSeed(u32_t seed);
		
	private:

		static std::random_device device;
		static std::default_random_engine generator;

	};

}