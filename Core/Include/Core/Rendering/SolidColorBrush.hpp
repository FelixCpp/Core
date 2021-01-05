#pragma once

#include <d2d1.h>
#include <wrl/client.h>

#include <Core/Rendering/Color.hpp>

namespace Core
{

	class GraphicsContext;

	class SolidColorBrush {
	public:

		explicit SolidColorBrush(GraphicsContext * gctx);

		void setColor(const Color & color);
		const Color & getColor() const;
		
		ID2D1Brush * getBrush() const;

	private:

		void create(const Color & color);

	private:

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
		Color color;

		GraphicsContext * gctx;

	};

}