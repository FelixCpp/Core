#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Maths/Vector2.hpp>
#include <Core/Rendering/Color.hpp>

#include <Core/System/LateRef.hpp>

/// <summary>
/// Direct2D Content
/// </summary>
#include <d2d1.h>
#include <wrl/client.h>

/// <summary>
/// C++ / STL
/// </summary>
#include <memory>
#include <vector>

namespace Core
{

	class Renderer;

	class RadialGradientBrush {
	public:

		explicit RadialGradientBrush(LateRef<Renderer> renderer);

		void SetCenter(float x, float y);
		const FVector2 & GetCenter() const;

		void SetRadius(float x, float y);
		const FVector2 & GetRadius() const;
		
		void SetOffset(float x, float y);
		const FVector2 & GetOffset() const;
		
		void SetColors(const std::vector<Color> & colors);
		const std::vector<Color> & GetColors();

		ID2D1Brush * GetBrush() const;

	private:
		
		void UpdateCenter();
		void UpdateRadius();
		void UpdateOffset();
		void UpdateColors();

	private:

		Microsoft::WRL::ComPtr<ID2D1RadialGradientBrush> brush;
		Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> collection;

		FVector2 center;
		FVector2 radius;
		FVector2 offset;

		std::vector<Color> colors;

		LateRef<Renderer> renderer;

	};

}