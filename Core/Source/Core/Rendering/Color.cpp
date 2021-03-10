#include <Core/Rendering/Color.hpp>

#include <Core/Maths/Random.hpp>

namespace Core
{

	const Color Color::White(255, 255, 255), Color::Black(0, 0, 0), Color::Clear(0, 0, 0, 0);
	const Color Color::LightBlue(0, 122, 255), Color::DarkBlue(10, 132, 255);
	const Color Color::LightGreen(52, 199, 89), Color::DarkGreen(48, 209, 88);
	const Color Color::LightIndigo(88, 86, 214), Color::DarkIndigo(94, 92, 230);
	const Color Color::LightOrange(255, 149, 0), Color::DarkOrange(255, 159, 10);
	const Color Color::LightPink(255, 45, 85), Color::DarkPink(255, 55, 95);
	const Color Color::LightPurple(175, 82, 222), Color::DarkPurple(191, 90, 242);
	const Color Color::LightRed(255, 59, 48), Color::DarkRed(255, 69, 58);
	const Color Color::LightTeal(90, 200, 250), Color::DarkTeal(100, 210, 255);
	const Color Color::LightYellow(255, 204, 0), Color::DarkYellow(255, 214, 10);
	const Color Color::LightGray1(142, 142, 147), Color::DarkGray1(142, 142, 147);
	const Color Color::LightGray2(174, 174, 178), Color::DarkGray2(99, 99, 102);
	const Color Color::LightGray3(199, 199, 204), Color::DarkGray3(72, 72, 74);
	const Color Color::LightGray4(209, 209, 214), Color::DarkGray4(58, 58, 60);
	const Color Color::LightGray5(299, 299, 234), Color::DarkGray5(44, 44, 46);
	const Color Color::LightGray6(242, 242, 247), Color::DarkGray6(28, 28, 30);

	Color::Color() :
		b(255), g(255), r(255), a(255)
	{
	}

	Color::Color(UInt8 red, UInt8 green, UInt8 blue, UInt8 alpha) :
		b(blue), g(green), r(red), a(alpha)
	{
	}

	Color::Color(Int32 rgb, UInt8 alpha) :
		b((rgb & 0x0000FF) >>  0),
		g((rgb & 0x00FF00) >>  8),
		r((rgb & 0xFF0000) >> 16),
		a(alpha)
	{
	}

	Color::Color(UInt32 rgba) :
		b((rgba & 0x0000FF00) >> 8),
		g((rgba & 0x00FF0000) >> 16),
		r((rgba & 0xFF000000) >> 24),
		a((rgba & 0x000000FF) >> 0)
	{
	}

	Color Color::Opacity(UInt8 alpha) const
	{
		return Color(this->r, this->g, this->b, alpha);
	}

	bool Color::operator==(const Color & other) const
	{
		return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
	}

	bool Color::operator!=(const Color & other) const
	{
		return !(*this == other);
	}

	UInt32 Color::Rgba() const
	{
		return (this->r << 24) | (this->g << 16) | (this->b << 8) | (this->a << 0);
	}

	UInt32 Color::Argb() const
	{
		return (this->a << 24) | (this->r << 16) | (this->g << 8) | (this->b << 0);
	}

	Int32 Color::Rgb() const
	{
		return (this->r << 16) | (this->g << 8) | (this->b << 0);
	}

	UInt8 Color::Brightness() const
	{
		return (UInt8)(0.2126f * this->r) + (UInt8)(0.7152f * this->g) + (UInt8)(0.0722f * this->b);
	}

	Color Color::GetRandomColor(UInt8 minValue, UInt8 maxValue, bool randomAlpha)
	{
		return Color(
			Random::Get<int>(minValue, maxValue),
			Random::Get<int>(minValue, maxValue),
			Random::Get<int>(minValue, maxValue),
			randomAlpha ? Random::Get<int>(minValue, maxValue) : 255
		);
	}

}