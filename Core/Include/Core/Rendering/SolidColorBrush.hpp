#pragma once

#include <d2d1.h>
#include <wrl/client.h>

#include <Core/System/DataTypes.hpp>

namespace Core
{

	class GraphicsContext;

	class SolidColorBrush {
	public:

		explicit SolidColorBrush(GraphicsContext * gctx);

		void setColor(const color_t & color);
		color_t getColor() const;
		
		ID2D1Brush * getBrush() const;

	private:

		void create(const color_t & color);

	private:

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
		color_t color;

		GraphicsContext * gctx;

	};

}