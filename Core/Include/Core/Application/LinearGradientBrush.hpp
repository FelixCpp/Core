#pragma once

#include <Core/System/DataTypes.hpp>
#include <Core/Maths/Vector2.hpp>

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

		void setColors(const std::vector<color_t> & colors);
		const std::vector<color_t> & getColors() const;

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

		std::vector<color_t> colors;

		GraphicsContext * gctx;

	};

}