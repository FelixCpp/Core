#pragma once

#include <Core/Maths/Vector2.hpp>
#include <Core/Rendering/Color.hpp>

#include <d2d1.h>
#include <wrl/client.h>

#include <memory>
#include <vector>

namespace Core
{

	class GraphicsContext;

	class RadialGradientBrush {
	public:

		explicit RadialGradientBrush(GraphicsContext *& gctx);

		void setCenter(float x, float y);
		const FVector2 & getCenter() const;

		void setRadius(float x, float y);
		const FVector2 & getRadius() const;
		
		void setOffset(float x, float y);
		const FVector2 & getOffset() const;
		
		void setColors(const std::vector<Color> & colors);
		const std::vector<Color> & getColors();

		ID2D1Brush * getBrush() const;

	private:
		
		void updateCenter();
		void updateRadius();
		void updateOffset();
		void updateColors();

	private:

		Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush> brush;
		Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> collection;

		FVector2 center;
		FVector2 radius;
		FVector2 offset;

		std::vector<Color> colors;

		GraphicsContext *& gctx;

	};

}