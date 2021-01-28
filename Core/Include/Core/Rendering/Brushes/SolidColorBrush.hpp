#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Color.hpp>
#include <Core/System/LateRef.hpp>

/// <summary>
/// Direct2D Content
/// </summary>
#include <d2d1.h>
#include <wrl/client.h>

namespace Core
{

	class Renderer;

	class SolidColorBrush {
	public:

		explicit SolidColorBrush(LateRef<Renderer> renderer);

		void SetColor(const Color & color);
		const Color & GetColor() const;
		
		ID2D1Brush * GetBrush() const;

	private:

		void Create(const Color & color);

	private:

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
		Color color;

		LateRef<Renderer> renderer;

	};

}