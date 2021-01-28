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

	class LinearGradientBrush {
	public:

		/// <summary>
		/// constructor which passes the Renderer into
		/// its member variable
		/// </summary>
		/// <param name="renderer">the Renderer</param>
		explicit LinearGradientBrush(LateRef<Renderer> renderer);

		/// <summary>
		/// Changes the starting point
		/// </summary>
		/// <param name="x">start x position in pixel space</param>
		/// <param name="y">start y position in pixel space</param>
		void SetStart(float x, float y);

		/// <summary>
		/// Returns the starting position
		/// 
		/// <remarks>see documentation SetStart()</remarks>
		/// </summary>
		const FVector2 & GetStart() const;

		/// <summary>
		/// Changes the end point
		/// </summary>
		/// <param name="x">end x position in pixel space</param>
		/// <param name="y">end y position in pixel space</param>
		void SetEnd(float x, float y);

		/// <summary>
		/// Returns the end position
		/// 
		/// <remarks>see documentation SetStart()</remarks>
		/// </summary>
		const FVector2 & GetEnd() const;

		/// <summary>
		/// Changes the colors of the gradient which
		/// gets drawn to the screen
		/// </summary>
		/// <param name="colors">the new colors</param>
		void SetColors(const std::vector<Color> & colors);

		/// <summary>
		/// Returns the colors of the gradient
		/// </summary>
		const std::vector<Color> & GetColors() const;

		/// <summary>
		/// Returns the Direct2D brush
		/// </summary>
		ID2D1Brush * GetBrush() const;

	private:

		/// <summary>
		/// Updates the starting
		/// point
		/// </summary>
		void UpdateStart();

		/// <summary>
		/// Updates the end position
		/// </summary>
		void UpdateEnd();

		/// <summary>
		/// Updates the colors
		/// </summary>
		void UpdateBrush();

	private:

		/// <summary>
		/// Smartpointer holding raw Direct2D-brush
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> brush;
		
		/// <summary>
		/// Smartpointer holding raw Direct2D GradientStopCollection
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1GradientStopCollection> collection;

		/// <summary>
		/// Starting point in pixel space
		/// </summary>
		FVector2 start;

		/// <summary>
		/// end point in pixel space
		/// </summary>
		FVector2 end;

		/// <summary>
		/// Colors used by the gradient
		/// </summary>
		std::vector<Color> colors;

		/// <summary>
		/// The Renderer holding the
		/// RenderTarget which is needed
		/// to create the GradientStopCollection &
		/// GradientBrush
		/// </summary>
		LateRef<Renderer> renderer;

	};

}