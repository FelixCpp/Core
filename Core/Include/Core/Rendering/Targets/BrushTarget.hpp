#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Color.hpp>
#include <Core/Rendering/ExtendMode.hpp>
#include <Core/Rendering/ImageInterpolationMode.hpp>

#include <Core/System/LateRef.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <vector>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class Image;
	class RenderState;
	class RenderStateManager;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// filling / outlining
	/// geometries of all kinds
	/// </summary>
	class BrushTarget {
	public:

		/// <summary>
		/// constructor which passes the RenderStateManager
		/// to its member variables.
		/// 
		/// Its needed to get access to the Shape attribute
		/// in the active RenderState
		/// </summary>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState which holds the shape object</param>
		explicit BrushTarget(LateRef<RenderStateManager> rsm);

		/// <summary>
		/// A virtual destructor
		/// </summary>
		virtual ~BrushTarget() = default;

		/// <summary>
		/// Changes the stroke weight
		/// </summary>
		/// <param name="strokeWeight">the thickness of the strokeweight in pixels</param>
		void StrokeWeight(float strokeWeight);

		/// <summary>
		/// Deactivates the fill.
		/// </summary>
		void NoFill();

		/// <summary>
		/// Deactives the outline
		/// </summary>
		void NoStroke();

		/// <summary>
		/// Changes and activates
		/// the solid-fill brush
		/// </summary>
		/// <param name="color">the new color</param>
		void Fill(const Color & color);

		/// <summary>
		/// Changes and activates
		/// the solid-outline brush
		/// </summary>
		/// <param name="color">the new color</param>
		void Stroke(const Color & color);

		/// <summary>
		/// Changes and activates
		/// the linear gradient brush
		/// as fill.
		/// </summary>
		/// <param name="colors">the colors to go through</param>
		/// <param name="startX">start point x in pixel space</param>
		/// <param name="startY">start point y in pixel space</param>
		/// <param name="endX">end point x in pixel space</param>
		/// <param name="endY">end point y in pixel space</param>
		void LinearFill(const std::vector<Color> & colors, float startX, float startY, float endX, float endY);

		/// <summary>
		/// Changes and activates
		/// the linear gradient brush
		/// as outline
		/// </summary>
		/// <param name="colors">the colors to go through</param>
		/// <param name="startX">start point x in pixel space</param>
		/// <param name="startY">start point y in pixel space</param>
		/// <param name="endX">end point x in pixel space</param>
		/// <param name="endY">end point y in pixel space</param>
		void LinearStroke(const std::vector<Color> & colors, float startX, float startY, float endX, float endY);

		/// <summary>
		/// Changes and activates
		/// the radial gradient brush
		/// as fill
		/// </summary>
		/// <param name="colors">the colors to go through</param>
		/// <param name="centerX">center point x in pixel space</param>
		/// <param name="centerY">center point y in pixel space</param>
		/// <param name="radiusX">radius x in pixel space</param>
		/// <param name="radiusY">radius y in pixel space</param>
		/// <param name="offsetX">offset x from the center x in pixel space</param>
		/// <param name="offsetY">offset y from the center y in pixel space</param>
		void RadialFill(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);

		/// <summary>
		/// Changes and activates
		/// the radial gradient brush
		/// as outline
		/// </summary>
		/// <param name="colors">the colors to go through</param>
		/// <param name="centerX">center point x in pixel space</param>
		/// <param name="centerY">center point y in pixel space</param>
		/// <param name="radiusX">radius x in pixel space</param>
		/// <param name="radiusY">radius y in pixel space</param>
		/// <param name="offsetX">offset x from the center x in pixel space</param>
		/// <param name="offsetY">offset y from the center y in pixel space</param>
		void RadialStroke(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);

		/// <summary>
		/// Changes and activates
		/// the bitmap brush
		/// as fill
		/// </summary>
		/// <param name="image">the image to use filling the area</param>
		/// <param name="mode">interpolation mode of the image</param>
		/// <param name="modeX">extend mode (horizontal)</param>
		/// <param name="modeY">extend mode (vertical)</param>
		/// <param name="opacity">opacity of the image</param>
		void ImageFill(const Image & image, ImageInterpolationMode mode = ImageInterpolationMode::NearestNeighbor, ExtendMode modeX = ExtendMode::Clamp, ExtendMode modeY = ExtendMode::Clamp, i32_t opacity = 255);
		
		/// <summary>
		/// Changes and activates
		/// the bitmap brush
		/// as outline
		/// </summary>
		/// <param name="image">the image to use filling the area</param>
		/// <param name="mode">interpolation mode of the image</param>
		/// <param name="modeX">extend mode (horizontal)</param>
		/// <param name="modeY">extend mode (vertical)</param>
		/// <param name="opacity">opacity of the image</param>
		void ImageStroke(const Image & image, ImageInterpolationMode mode = ImageInterpolationMode::NearestNeighbor, ExtendMode modeX = ExtendMode::Clamp, ExtendMode modeY = ExtendMode::Clamp, i32_t opacity = 255);

	private:

		/// <summary>
		/// returns the current shape of the
		/// active RenderState
		/// </summary>
		RenderState & GetState();

	private:

		/// <summary>
		/// An instance of the RenderStateManager
		/// to get access to the brushes / drawing
		/// properties
		/// </summary>
		LateRef<RenderStateManager> rsm;

	};

	/// <summary>
	/// Example:
	/// <code>
	/// 
	/// ...
	/// Fill(Color(182, 40, 210));
	/// Rect(200.f, 200.f, 100.f, 100.f);
	/// 
	/// Stroke(0xFF00A0FF);
	/// Ellipse(350.f, 200.f, 50.f, 50.f);
	/// ...
	/// 
	/// </code>
	/// </summary>

}