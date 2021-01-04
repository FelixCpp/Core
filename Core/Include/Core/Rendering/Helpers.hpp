#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/System/StaticObject.hpp>
#include <Core/Maths/Constants.hpp>

namespace Core
{

	constexpr color_t color(i32_t red, i32_t green, i32_t blue, i32_t alpha = 255)
	{
		return (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
	}

	constexpr color_t color(i32_t grey, i32_t alpha = 255)
	{
		return color(grey, grey, grey, alpha);
	}

	constexpr i32_t red(color_t argb)
	{
		return (argb & 0x00FF0000) >> 16;
	}

	constexpr i32_t green(color_t argb)
	{
		return (argb & 0x0000FF00) >> 8;
	}

	constexpr i32_t blue(color_t argb)
	{
		return (argb & 0x000000FF) >> 0;
	}

	constexpr i32_t alpha(color_t argb)
	{
		return (argb & 0xFF000000) >> 24;
	}

	constexpr i32_t brightness(color_t argb)
	{
		const i32_t r = red(argb);
		const i32_t g = green(argb);
		const i32_t b = blue(argb);

		return (i32_t)(0.2126f * r) + (i32_t)(0.7152f * g) + (i32_t)(0.0722f * b);
	}	

	constexpr float radians(float degrees)
	{
		return degrees * (float)PI / 180.f;
	}

	constexpr float degrees(float radians)
	{
		return radians * 180.f / (float)PI;
	}

	float random(float min, float max);
	float random(float max);

	struct Color : StaticObject {
		enum : color_t {
			White = color(255, 255, 255), Black = color(0, 0, 0), Clear = color(0, 0, 0, 0),
			LightBlue = color(0, 122, 255), DarkBlue = color(10, 132, 255),
			LightGreen = color(52, 199, 89), DarkGreen = color(48, 209, 88),
			LightIndigo = color(88, 86, 214), DarkIndigo = color(94, 92, 230),
			LightOrange = color(255, 149, 0), DarkOrange = color(255, 159, 10),
			LightPink = color(255, 45, 85), DarkPink = color(255, 55, 95),
			LightPurple = color(175, 82, 222), DarkPurple = color(191, 90, 242),
			LightRed = color(255, 59, 48), DarkRed = color(255, 69, 58),
			LightTeal = color(90, 200, 250), DarkTeal = color(100, 210, 255),
			LightYellow = color(255, 204, 0), DarkYellow = color(255, 214, 10),
			LightGray1 = color(142, 142, 147), DarkGray1 = color(142, 142, 147),
			LightGray2 = color(174, 174, 178), DarkGray2 = color(99, 99, 102),
			LightGray3 = color(199, 199, 204), DarkGray3 = color(72, 72, 74),
			LightGray4 = color(209, 209, 214), DarkGray4 = color(58, 58, 60),
			LightGray5 = color(299, 299, 234), DarkGray5 = color(44, 44, 46),
			LightGray6 = color(242, 242, 247), DarkGray6 = color(28, 28, 30)
		};
	};

}