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

	class LinearGradientBrush {
	public:

		explicit LinearGradientBrush(GraphicsContext * gctx);

		void setStart(float x, float y);
		const FVector2 & getStart() const;

		void setEnd(float x, float y);
		const FVector2 & getEnd() const;

		void setColors(const std::vector<Color> & colors);
		const std::vector<Color> & getColors() const;

		ID2D1Brush * getBrush() const;

	private:

		void updateStart();
		void updateEnd();
		void updateBrush();

	private:

		Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> brush;
		Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> collection;

		FVector2 start;
		FVector2 end;

		std::vector<Color> colors;

		GraphicsContext * gctx;

	};

}