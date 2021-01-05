#include <Core/Rendering/LinearGradientBrush.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/Helpers.hpp>

#include <Core/System/Logger.hpp>

#include <iostream>

namespace Core
{

	LinearGradientBrush::LinearGradientBrush(GraphicsContext * gctx) :
		brush(nullptr),
		collection(nullptr),
		start(0.f, 0.f),
		end(0.f, 0.f),
		colors(),
		gctx(gctx)
	{
	}
	
	void LinearGradientBrush::setStart(float x, float y)
	{
		if (this->start.x != x || this->start.y != y)
		{
			this->start.x = x;
			this->start.y = y;

			this->updateStart();
		}
	}

	const FVector2 & LinearGradientBrush::getStart() const
	{
		return this->start;
	}

	void LinearGradientBrush::setEnd(float x, float y)
	{
		if (this->end.x != x || this->end.y != y)
		{
			this->end.x = x;
			this->end.y = y;

			this->updateEnd();
		}
	}

	const FVector2 & LinearGradientBrush::getEnd() const
	{
		return this->end;
	}

	void LinearGradientBrush::setColors(const std::vector<color_t> & colors)
	{
		if (this->colors != colors)
		{
			this->colors = colors;
			this->updateBrush();
		}
	}

	const std::vector<color_t> & LinearGradientBrush::getColors() const
	{
		return this->colors;
	}

	ID2D1Brush * LinearGradientBrush::getBrush() const
	{
		return this->brush.Get();
	}

	void LinearGradientBrush::updateStart()
	{
		if (ID2D1LinearGradientBrush * brush = this->brush.Get())
			brush->SetStartPoint(D2D1::Point2F(this->start.x, this->start.y));
	}

	void LinearGradientBrush::updateEnd()
	{
		if (ID2D1LinearGradientBrush * brush = this->brush.Get())
			brush->SetEndPoint(D2D1::Point2F(this->end.x, this->end.y));
	}

	void LinearGradientBrush::updateBrush()
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

		hr = rt->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(this->start.x, this->start.y),
				D2D1::Point2F(this->end.x, this->end.y)
			),
			this->collection.Get(),
			&this->brush
		);
		if (FAILED(hr))
		{
			CORE_ERROR("CreateLinearGradientBrush");
			return;
		}
	}

}