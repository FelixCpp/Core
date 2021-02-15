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

		/// <summary>
		/// Creates a white color. R, G, B and A will be assignedd to 255
		/// </summary>
		Color();

		/// <summary>
		/// Copies the parameters into their member variables
		/// </summary>
		/// <param name="red">r component</param>
		/// <param name="green">g component</param>
		/// <param name="blue">b component</param>
		/// <param name="alpha">a component</param>
		Color(u8_t red, u8_t green, u8_t blue, u8_t alpha = 255);

		/// <summary>
		/// creates a colors based on the parameters
		/// </summary>
		/// <param name="rgb">32-bit integer in R-G-B format</param>
		/// <param name="alpha">alpha component</param>
		Color(i32_t rgb, u8_t alpha);

		/// <summary>
		/// creates a colors based on the parameters
		/// </summary>
		/// <param name="rgba">32-bit unsigned integer in R-G-B-A format</param>
		Color(u32_t rgba);

		/// <summary>
		/// Returns the current color modified by the alpha
		/// component.
		/// </summary>
		/// 
		/// <example>
		/// Color yellow = Color::Yellow.Opacity(150);
		/// </example>
		Color Opacity(u8_t alpha) const;

		/// <summary>
		/// Checks for equality and returns the result
		/// </summary>
		bool operator==(const Color & other) const;

		/// <summary>
		/// Checks for differences and returns the result
		/// </summary>
		bool operator!=(const Color & other) const;

		/// <summary>
		/// calculates a 32-bit unsigned integer value in R-G-B-A format
		/// and returns it.
		/// </summary>
		u32_t Rgba() const;

		/// <summary>
		/// calculates a 32-bit unsigned integer value in A-R-G-B format
		/// and returns it.
		/// </summary>
		u32_t Argb() const;

		/// <summary>
		/// calculates a 32-bit integer value in R-G-B format
		/// and returns it.
		/// </summary>
		i32_t Rgb() const;

		/// <summary>
		/// Calculates the brightness value
		/// using the red, green and blue
		/// components
		/// </summary>
		u8_t Brightness() const;

		/// <summary>
		/// returns a random color.
		/// Each value will have its own
		/// random 0-255 value
		/// </summary>
		/// <param name="randomAlpha">a boolean to indicate wether the alpha component should have a random value or 255 assigned to itor </param>
		static Color GetRandomColor(bool randomAlpha = false);

		/// <summary>
		///	don't change the order of this attributes !
		///	this order is important to insert them efficiently
		///	into an image
		/// </summary>

		/// <summary>
		/// blue component
		/// </summary>
		u8_t b;

		/// <summary>
		/// green component
		/// </summary>
		u8_t g;

		/// <summary>
		/// red component
		/// </summary>
		u8_t r;

		/// <summary>
		/// alpha component
		/// </summary>
		u8_t a;

	};

}