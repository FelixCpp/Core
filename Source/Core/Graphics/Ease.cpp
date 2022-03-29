// 
// Ease.cpp
// Core
// 
// Created by Felix Busch on 29.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/Animatable.hpp>

#include <cmath>
#include <numbers>

#ifndef PI
#define PI std::numbers::pi_v<float>

namespace Core::Ease
{

	f32 Linear(f32 x)
	{
		return x;
	}

	f32 InSine(f32 x)
	{
		return 1.0f - std::cos((x * PI) / 2.0f);
	}

	f32 OutSine(f32 x)
	{
		return std::sin((x * PI) / 2.0f);
	}

	f32 InOutSine(f32 x)
	{
		return -(std::cos(PI * x) - 1.0f) / 2.0f;
	}

	f32 InCubic(f32 x)
	{
		return x * x * x;
	}

	f32 OutCubic(f32 x)
	{
		return 1.0f - std::pow(1.0f - x, 3.0f);
	}

	f32 InOutCubic(f32 x)
	{
		return x < 0.5f ? 4.0f * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f;
	}

	f32 InQuint(f32 x)
	{
		return x * x * x * x * x;
	}

	f32 OutQuint(f32 x)
	{
		return 1.0f - std::pow(1.0f - x, 5.0f);
	}

	f32 InOutQuint(f32 x)
	{
		return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 5.0f) / 2.0f;
	}

	f32 InCirc(f32 x)
	{
		return 1.0f - std::sqrt(1.0f - std::pow(x, 2.0f));
	}

	f32 OutCirc(f32 x)
	{
		return std::sqrt(1.0f - std::pow(x - 1.0f, 2.0f));
	}

	f32 InOutCirc(f32 x)
	{
		return x < 0.5f
			? (1.0f - std::sqrt(1.0f - std::pow(2.0f * x, 2.0f))) / 2.0f
			: (std::sqrt(1.0f - std::pow(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
	}

	f32 InElastic(f32 x)
	{
		static constexpr f32 c4 = (2.0f * PI) / 3.0f;

		return x == 0.0f
			? 0.0f
			: x == 1.0f
			? 1.0f
			: -std::pow(2.0f, 10.0f * x - 10.0f) * std::sin((x * 10.0f - 10.75f) * c4);
	}

	f32 OutElastic(f32 x)
	{
		static constexpr f32 c4 = (2.0f * PI) / 3.0f;

		return x == 0.0f
			? 0.0f
			: x == 1.0f
			? 1.0f
			: pow(2.0f, -10.0f * x) * sin((x * 10.0f - 0.75f) * c4) + 1.0f;
	}

	f32 InOutElastic(f32 x)
	{
		static constexpr f32 c5 = (2.0f * PI) / 4.5f;

		return x == 0.0f
			? 0.0f
			: x == 1.0f
			? 1.0f
			: x < 0.5f
			? -(std::pow(2.0f, 20.0f * x - 10.0f) * std::sin((20.0f * x - 11.125f) * c5)) / 2.0f
			: (std::pow(2.0f, -20.0f * x + 10.0f) * std::sin((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
	}

	f32 InQuad(f32 x)
	{
		return x * x;
	}

	f32 OutQuad(f32 x)
	{
		return 1.0f - (1.0f - x) * (1.0f - x);
	}

	f32 InOutQuad(f32 x)
	{
		return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f;
	}

	f32 InQuart(f32 x)
	{
		return x * x * x * x;
	}

	f32 OutQuart(f32 x)
	{
		return 1.0f - std::pow(1.0f - x, 4.0f);
	}

	f32 InOutQuart(f32 x)
	{
		return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 4.0f) / 2.0f;
	}

	f32 InExpo(f32 x)
	{
		return x == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * x - 10.0f);
	}

	f32 OutExpo(f32 x)
	{
		return x == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * x);
	}

	f32 InOutExpo(f32 x)
	{
		return x == 0.0f
			? 0.0f
			: x == 1.0f
			? 1.0f
			: x < 0.5f ? pow(2.0f, 20.0f * x - 10.0f) / 2.0f
			: (2.0f - pow(2.0f, -20.0f * x + 10.0f)) / 2.0f;
	}

	f32 InBack(f32 x)
	{
		static constexpr f32 c1 = 1.70158f;
		static constexpr f32 c3 = c1 + 1.0f;

		return c3 * x * x * x - c1 * x * x;
	}

	f32 OutBack(f32 x)
	{
		static constexpr f32 c1 = 1.70158f;
		static constexpr f32 c3 = c1 + 1.0f;
		return 1.0f + c3 * std::pow(x - 1.0f, 3.0f) + c1 * std::pow(x - 1.0f, 2.0f);
	}

	f32 InOutBack(f32 x)
	{
		static constexpr f32 c1 = 1.70158f;
		static constexpr f32 c2 = c1 * 1.525f;

		return x < 0.5f
			? (std::pow(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
			: (std::pow(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	}

	f32 InBounce(f32 x)
	{
		return 1.0f - OutBounce(1.0f - x);
	}

	f32 OutBounce(f32 x)
	{
		static constexpr f32 n1 = 7.5625f;
		static constexpr f32 d1 = 2.75f;

		if (x < 1.0f / d1) {
			return n1 * x * x;
		}

		if (x < 2.0f / d1) {
			return n1 * (x - 1.5f / d1) * x + 0.75f;
		}

		if (x < 2.5f / d1) {
			return n1 * (x - 2.25f / d1) * x + 0.9375f;
		}

		return n1 * (x - 2.625f / d1) * x + 0.984375f;
	}

	f32 InOutBounce(f32 x)
	{
		return x < 0.5f
			? (1.0f - OutBounce(1.0f - 2.0f * x)) / 2.0f
			: (1.0f + OutBounce(2.0f * x - 1.0f)) / 2.0f;
	}
}

#undef PI
#endif