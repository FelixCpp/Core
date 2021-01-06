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

	Color::Color(u8_t red, u8_t green, u8_t blue, u8_t alpha) :
		b(blue), g(green), r(red), a(alpha)
	{
	}

	Color::Color(i32_t rgb, u8_t alpha) :
		b((rgb & 0x0000FF) >>  0),
		g((rgb & 0x00FF00) >>  8),
		r((rgb & 0xFF0000) >> 16),
		a(alpha)
	{
	}

	Color::Color(u32_t rgba) :
		b((rgba & 0x0000FF00) >> 8),
		g((rgba & 0x00FF0000) >> 16),
		r((rgba & 0xFF000000) >> 24),
		a((rgba & 0x000000FF) >> 0)
	{
	}

	bool Color::operator==(const Color & other) const
	{
		return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
	}

	bool Color::operator!=(const Color & other) const
	{
		return !(*this == other);
	}

	u32_t Color::rgba() const
	{
		return (this->r << 24) | (this->g << 16) | (this->b << 8) | (this->a << 0);
	}

	u32_t Color::argb() const
	{
		return (this->a << 24) | (this->r << 16) | (this->g << 8) | (this->b << 0);
	}

	i32_t Color::rgb() const
	{
		return (this->r << 16) | (this->g << 8) | (this->b << 0);
	}

	u8_t Color::brightness() const
	{
		return (u8_t)(0.2126f * this->r) + (u8_t)(0.7152f * this->g) + (u8_t)(0.0722f * this->b);
	}

	Color Color::getRandomColor(bool randomAlpha)
	{
		return Color(
			Core::Random::get(255),
			Core::Random::get(255),
			Core::Random::get(255),
			randomAlpha ? Core::Random::get(255) : 255
		);
	}

}