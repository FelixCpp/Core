#include <Core/Rendering/SolidColorBrush.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/Helpers.hpp>

#include <iostream>

namespace Core
{

	SolidColorBrush::SolidColorBrush(GraphicsContext * gctx) :
		brush(nullptr),
		color(Color::Clear),
		gctx(gctx)
	{
	}

	void SolidColorBrush::setColor(const color_t & color)
	{
		if (this->color != color)
		{
			this->color = color;

			if (this->brush)
			{
				this->brush->SetColor(D2D1::ColorF(
					(float)red(color)   / 255.f,
					(float)green(color) / 255.f,
					(float)blue(color)  / 255.f,
					(float)alpha(color) / 255.f
				));
			} else
			{
				this->create(color);
			}
		}
	}

	color_t SolidColorBrush::getColor() const
	{
		return this->color;
	}

	ID2D1Brush * SolidColorBrush::getBrush() const
	{
		return this->brush.Get();
	}

	void SolidColorBrush::create(const color_t & color)
	{
		HRESULT hr = this->gctx->hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(
			(float)red(color)   / 255.f,
			(float)green(color) / 255.f,
			(float)blue(color)  / 255.f,
			(float)alpha(color) / 255.f
		), &this->brush);
		if (FAILED(hr))
		{
			std::cerr << "CreateSolidColorBrush" << std::endl;
			return;
		}
	}

}