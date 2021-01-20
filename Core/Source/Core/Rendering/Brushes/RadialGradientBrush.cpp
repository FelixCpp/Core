#include <Core/Rendering/Brushes/RadialGradientBrush.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/Helpers.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	RadialGradientBrush::RadialGradientBrush(Renderer *& renderer) :
		brush(nullptr),
		collection(nullptr),
		center(0.f, 0.f),
		radius(0.f, 0.f),
		offset(0.f, 0.f),
		colors(),
		renderer(renderer)
	{
	}

	void RadialGradientBrush::SetCenter(float x, float y)
	{
		if (this->center.x != x || this->center.y != y)
		{
			this->center.x = x;
			this->center.y = y;
			this->UpdateCenter();
		}
	}

	const FVector2 & RadialGradientBrush::GetCenter() const
	{
		return this->center;
	}

	void RadialGradientBrush::SetRadius(float x, float y)
	{
		if (this->radius.x != x || this->radius.y != y)
		{
			this->radius.x = x;
			this->radius.y = y;
			this->UpdateRadius();
		}
	}

	const FVector2 & RadialGradientBrush::GetRadius() const
	{
		return this->radius;
	}

	void RadialGradientBrush::SetOffset(float x, float y)
	{
		if (this->offset.x != x || this->offset.y != y)
		{
			this->offset.x = x;
			this->offset.y = y;
			this->UpdateOffset();
		}
	}

	const FVector2 & RadialGradientBrush::GetOffset() const
	{
		return this->offset;
	}

	void RadialGradientBrush::SetColors(const std::vector<Color> & colors)
	{
		if (this->colors != colors)
		{
			this->colors = colors;
			this->UpdateColors();
		}
	}

	const std::vector<Color> & RadialGradientBrush::GetColors()
	{
		return this->colors;
	}

	ID2D1Brush * RadialGradientBrush::GetBrush() const
	{
		return this->brush.Get();
	}

	void RadialGradientBrush::UpdateCenter()
	{
		if (ID2D1RadialGradientBrush * brush = this->brush.Get())
			brush->SetCenter(D2D1::Point2F(this->center.x, this->center.y));
	}

	void RadialGradientBrush::UpdateRadius()
	{
		if (ID2D1RadialGradientBrush * brush = this->brush.Get())
		{
			brush->SetRadiusX(this->radius.x);
			brush->SetRadiusY(this->radius.y);
		}
	}

	void RadialGradientBrush::UpdateOffset()
	{
		if (ID2D1RadialGradientBrush * brush = this->brush.Get())
			brush->SetGradientOriginOffset(D2D1::Point2F(this->offset.x, this->offset.y));
	}

	void RadialGradientBrush::UpdateColors()
	{
		ID2D1RenderTarget * rt = this->renderer->GetRenderTarget();

		const size_t size = this->colors.size();
		std::vector<D2D1_GRADIENT_STOP> gradientStops(size);

		for (size_t i = 0; i < this->colors.size(); i++)
		{
			const Color & color = this->colors[i];

			gradientStops[i] = D2D1::GradientStop(
				(float)i / ((float)size - 1.f),
				D2D1::ColorF(
					(float)color.r / 255.f,
					(float)color.g / 255.f,
					(float)color.b / 255.f,
					(float)color.a / 255.f
				)
			);
		}

		HRESULT hr = rt->CreateGradientStopCollection(
			gradientStops.data(),
			(UINT32)gradientStops.size(),
			&this->collection
		);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a GradientStopCollection");
			return;
		}

		hr = rt->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(this->center.x, this->center.y),
				D2D1::Point2F(this->offset.x, this->offset.y),
				this->radius.x, this->radius.y
			),
			this->collection.Get(),
			&this->brush
		);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a RadialGradientBrush");
			return;
		}
	}

}