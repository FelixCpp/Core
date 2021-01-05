#pragma once

#include <Core/System/Datatypes.hpp>

namespace Core
{

	struct Color {

		/* some pre-defined colors from iOS */
		static const Color White, Black, Clear;
		static const Color LightBlue, DarkBlue;
		static const Color LightGreen, DarkGreen;
		static const Color LightIndigo, DarkIndigo;
		static const Color LightOrange, DarkOrange;
		static const Color LightPink, DarkPink;
		static const Color LightPurple, DarkPurple;
		static const Color LightRed, DarkRed;
		static const Color LightTeal, DarkTeal;
		static const Color LightYellow, DarkYellow;
		static const Color LightGray1, DarkGray1;
		static const Color LightGray2, DarkGray2;
		static const Color LightGray3, DarkGray3;
		static const Color LightGray4, DarkGray4;
		static const Color LightGray5, DarkGray5;
		static const Color LightGray6, DarkGray6;

		/* creates a white color. R, G, B and A will be assigned to 255 */
		constexpr Color();

		/* copies the parameters into the member variables */
		constexpr Color(u8_t red, u8_t green, u8_t blue, u8_t alpha = 255);

		/* takes in an 32-bit integer in an R-G-B format and the alpha component */
		constexpr Color(i32_t rgb, u8_t alpha = 255);

		/* takes in an 32-bit unsigned integer */
		constexpr Color(u32_t rgba);

		/* checks for equality */
		bool operator==(const Color & other) const;

		/* checks for differences */
		bool operator!=(const Color & other) const;

		/* calcualtes a 32-bit unsigned integer value in R-G-B-A format */
		constexpr u32_t rgba() const;

		/* calcualtes a 32-bit unsigned integer value in A-R-G-B format */
		constexpr u32_t argb() const;

		/* calculates a 32-bit integer value in R-G-B format */
		constexpr i32_t rgb() const;

		/* calculates a 8-bit unsigned integer holding the brightness value of the red, green and blue component */
		constexpr u8_t brightness() const;

		/* red component */
		u8_t r;

		/* green component */
		u8_t g;

		/* blue component */
		u8_t b;

		/* alpha component */
		u8_t a;

	};

}