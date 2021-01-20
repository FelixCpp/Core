#pragma once

#include <d2d1.h>
#include <wrl/client.h>

#include <Core/Rendering/Color.hpp>

namespace Core
{

	class Renderer;

	class SolidColorBrush {
	public:

		explicit SolidColorBrush(Renderer *& renderer);

		void SetColor(const Color & color);
		const Color & GetColor() const;
		
		ID2D1Brush * GetBrush() const;

	private:

		void Create(const Color & color);

	private:

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
		Color color;

		Renderer *& renderer;

	};

}