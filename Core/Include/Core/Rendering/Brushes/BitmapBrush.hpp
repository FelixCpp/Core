#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Image.hpp>
#include <Core/Rendering/ExtendMode.hpp>
#include <Core/Rendering/ImageInterpolationMode.hpp>

#include <Core/System/LateRef.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <memory>

/// <summary>
/// Ignore this
/// </summary>
struct ID2D1Brush;

namespace Core
{

	class Renderer;

	class BitmapBrush {
	public:

		explicit BitmapBrush(LateRef<Renderer> gctx);
		
		void SetImage(const Image & image);
		const Image & GetImage() const;

		void SetInterpolationMode(ImageInterpolationMode mode);
		ImageInterpolationMode GetInterpolationMode() const;

		void SetExtendModeX(ExtendMode modeX);
		ExtendMode GetExtendModeX() const;

		void SetExtendModeY(ExtendMode modeY);
		ExtendMode GetExtendModeY() const;

		ID2D1Brush * GetBrush() const;

	private:

		struct Implementation;
		std::shared_ptr<Implementation> impl;

		LateRef<Renderer> renderer;

		Image image;
		ImageInterpolationMode interpolationMode;

		ExtendMode modeX;
		ExtendMode modeY;
		
	};

}