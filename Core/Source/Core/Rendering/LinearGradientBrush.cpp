#include <Core/Rendering/LinearGradientBrush.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/Helpers.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	LinearGradientBrush::LinearGradientBrush(Renderer *& renderer) :
		brush(nullptr),
		collection(nullptr),
		start(0.f, 0.f),
		end(0.f, 0.f),
		colors(),
		renderer(renderer)
	{
	}
	
	void LinearGradientBrush::SetStart(float x, float y)
	{
		if (this->start.x != x || this->start.y != y)
		{
			this->start.x = x;
			this->start.y = y;

			this->UpdateStart();
		}
	}

	const FVector2 & LinearGradientBrush::GetStart() const
	{
		return this->start;
	}

	void LinearGradientBrush::SetEnd(float x, float y)
	{
		if (this->end.x != x || this->end.y != y)
		{
			this->end.x = x;
			this->end.y = y;

			this->UpdateEnd();
		}
	}

	const FVector2 & LinearGradientBrush::GetEnd() const
	{
		return this->end;
	}

	void LinearGradientBrush::SetColors(const std::vector<Color> & colors)
	{
		if (this->colors != colors)
		{
			this->colors = colors;
			this->UpdateBrush();
		}
	}

	const std::vector<Color> & LinearGradientBrush::GetColors() const
	{
		return this->colors;
	}

	ID2D1Brush * LinearGradientBrush::GetBrush() const
	{
		return this->brush.Get();
	}

	void LinearGradientBrush::UpdateStart()
	{
		if (ID2D1LinearGradientBrush * brush = this->brush.Get())
			brush->SetStartPoint(D2D1::Point2F(this->start.x, this->start.y));
	}

	void LinearGradientBrush::UpdateEnd()
	{
		if (ID2D1LinearGradientBrush * brush = this->brush.Get())
			brush->SetEndPoint(D2D1::Point2F(this->end.x, this->end.y));
	}

	void LinearGradientBrush::UpdateBrush()
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
			CORE_ERROR("Failed to create a LinearGradientBrush");
			return;
		}
	}

}