#include <Core/Rendering/SolidColorBrush.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/Helpers.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	SolidColorBrush::SolidColorBrush(GraphicsContext * gctx) :
		brush(nullptr),
		color(Color::Clear),
		gctx(gctx)
	{
	}

	void SolidColorBrush::setColor(const Color & color)
	{
		if (this->color != color)
		{
			this->color = color;

			if (this->brush)
			{
				this->brush->SetColor(D2D1::ColorF(
					(float)color.r / 255.f,
					(float)color.g / 255.f,
					(float)color.b / 255.f,
					(float)color.a / 255.f
				));
			} else
			{
				this->create(color);
			}
		}
	}

	const Color & SolidColorBrush::getColor() const
	{
		return this->color;
	}

	ID2D1Brush * SolidColorBrush::getBrush() const
	{
		return this->brush.Get();
	}

	void SolidColorBrush::create(const Color & color)
	{
		HRESULT hr = this->gctx->hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(
			(float)color.r / 255.f,
			(float)color.g / 255.f,
			(float)color.b / 255.f,
			(float)color.a / 255.f
		), &this->brush);
		if (FAILED(hr))
		{
			CORE_ERROR("CreateSolidColorBrush");
			return;
		}
	}

}