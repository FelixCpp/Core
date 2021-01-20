#include <Core/Rendering/SolidColorBrush.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/Helpers.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	SolidColorBrush::SolidColorBrush(Renderer *& renderer) :
		brush(nullptr),
		color(Color::Clear),
		renderer(renderer)
	{
	}

	void SolidColorBrush::SetColor(const Color & color)
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
				this->Create(color);
			}
		}
	}

	const Color & SolidColorBrush::GetColor() const
	{
		return this->color;
	}

	ID2D1Brush * SolidColorBrush::GetBrush() const
	{
		return this->brush.Get();
	}

	void SolidColorBrush::Create(const Color & color)
	{
		if (this->renderer)
		{
			HRESULT hr = this->renderer->GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(
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

}