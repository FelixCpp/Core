#pragma once

#include <memory>
#include <Core/Rendering/Image.hpp>
#include <Core/Rendering/ExtendMode.hpp>
#include <Core/Rendering/ImageInterpolationMode.hpp>

struct ID2D1Brush;

namespace Core
{

	class GraphicsContext;

	class BitmapBrush {
	public:

		explicit BitmapBrush(GraphicsContext *& gctx);
		
		void SetImage(const Image & image);
		const Image & GetImage() const;

		void SetInterpolationMode(ImageInterpolationMode mode);
		ImageInterpolationMode GetInterpolationMode() const;

		void SetExtendModeX(ExtendMode modeX);
		ExtendMode GetExtendModeX() const;

		void SetExtendModeY(ExtendMode modeY);
		ExtendMode GetExtendModeY() const;

		void SetOpacity(i32_t opacity);
		i32_t GetOpacity() const;

		ID2D1Brush * GetBrush() const;

	private:

		struct Implementation;
		std::shared_ptr<Implementation> impl;

		GraphicsContext *& gctx;

		Image image;
		ImageInterpolationMode interpolationMode;

		ExtendMode modeX;
		ExtendMode modeY;
		
		i32_t opacity;

	};

}