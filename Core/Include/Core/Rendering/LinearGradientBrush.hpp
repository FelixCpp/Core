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

		explicit LinearGradientBrush(GraphicsContext *& gctx);

		void SetStart(float x, float y);
		const FVector2 & GetStart() const;

		void SetEnd(float x, float y);
		const FVector2 & GetEnd() const;

		void SetColors(const std::vector<Color> & colors);
		const std::vector<Color> & GetColors() const;

		ID2D1Brush * GetBrush() const;

	private:

		void UpdateStart();
		void UpdateEnd();
		void UpdateBrush();

	private:

		Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> brush;
		Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> collection;

		FVector2 start;
		FVector2 end;

		std::vector<Color> colors;

		GraphicsContext *& gctx;

	};

}