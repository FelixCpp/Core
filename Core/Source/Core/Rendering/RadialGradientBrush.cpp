#include <Core/Rendering/RadialGradientBrush.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/Helpers.hpp>

#include <Core/System/Logger.hpp>

#include <iostream>

namespace Core
{

	RadialGradientBrush::RadialGradientBrush(GraphicsContext * gctx) :
		brush(nullptr),
		collection(nullptr),
		center(0.f, 0.f),
		radius(0.f, 0.f),
		offset(0.f, 0.f),
		colors(),
		gctx(gctx)
	{
	}

	void RadialGradientBrush::setCenter(float x, float y)
	{
		if (this->center.x != x || this->center.y != y)
		{
			this->center.x = x;
			this->center.y = y;
			this->updateCenter();
		}
	}

	const FVector2 & RadialGradientBrush::getCenter() const
	{
		return this->center;
	}

	void RadialGradientBrush::setRadius(float x, float y)
	{
		if (this->radius.x != x || this->radius.y != y)
		{
			this->radius.x = x;
			this->radius.y = y;
			this->updateRadius();
		}
	}

	const FVector2 & RadialGradientBrush::getRadius() const
	{
		return this->radius;
	}

	void RadialGradientBrush::setOffset(float x, float y)
	{
		if (this->offset.x != x || this->offset.y != y)
		{
			this->offset.x = x;
			this->offset.y = y;
			this->updateOffset();
		}
	}

	const FVector2 & RadialGradientBrush::getOffset() const
	{
		return this->offset;
	}

	void RadialGradientBrush::setColors(const std::vector<color_t> & colors)
	{
		if (this->colors != colors)
		{
			this->colors = colors;
			this->updateColors();
		}
	}

	const std::vector<color_t> & RadialGradientBrush::getColors()
	{
		return this->colors;
	}

	ID2D1Brush * RadialGradientBrush::getBrush() const
	{
		return this->brush.Get();
	}

	void RadialGradientBrush::updateCenter()
	{
		if (ID2D1RadialGradientBrush * brush = this->brush.Get())
			brush->SetCenter(D2D1::Point2F(this->center.x, this->center.y));
	}

	void RadialGradientBrush::updateRadius()
	{
		if (ID2D1RadialGradientBrush * brush = this->brush.Get())
		{
			brush->SetRadiusX(this->radius.x);
			brush->SetRadiusY(this->radius.y);
		}
	}

	void RadialGradientBrush::updateOffset()
	{
		if (ID2D1RadialGradientBrush * brush = this->brush.Get())
			brush->SetGradientOriginOffset(D2D1::Point2F(this->offset.x, this->offset.y));
	}

	void RadialGradientBrush::updateColors()
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();

		const size_t size = this->colors.size();
		std::vector<D2D1_GRADIENT_STOP> gradientStops(size);

		for (size_t i = 0; i < this->colors.size(); i++)
		{
			const color_t & color = this->colors[i];

			gradientStops[i] = D2D1::GradientStop(
				(float)i / ((float)size - 1.f),
				D2D1::ColorF(
					(float)red(color)   / 255.f,
					(float)green(color) / 255.f,
					(float)blue(color)  / 255.f,
					(float)alpha(color) / 255.f
				)
			);
		}

		HRESULT hr = rt->CreateGradientStopCollection(
			gradientStops.data(),
			gradientStops.size(),
			&this->collection
		);
		if (FAILED(hr))
		{
			CORE_ERROR("CreateGradientStopCollection");
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
			CORE_ERROR("CreateRadialGradientBrush");
			return;
		}
	}

}