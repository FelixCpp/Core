#pragma once

#include <Core/Rendering/DrawMode.hpp>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class RenderState;
	class RenderStateManager;
	class Renderer;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// primitive drawings
	/// </summary>
	class PrimitiveRenderTarget {
	public:

		/// <summary>
		/// constructor which passes the Renderer & RenderStateManager
		/// to its member variables.
		/// </summary>
		/// <param name="renderer">the Renderer which is needed to draw the primitives</param>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState holding informations how the primitives should be drawn</param>
		explicit PrimitiveRenderTarget(Renderer *& renderer, RenderStateManager *& rsm);

		/// <summary>
		/// A virtual destructor
		/// </summary>
		virtual ~PrimitiveRenderTarget() = default;

		/// <summary>
		/// Changes the RectMode which
		/// is used by the Rect(...) method
		/// 
		/// <remarks>see documentation for DrawMode</remarks>
		/// </summary>
		/// <param name="mode">the rectmode to use</param>
		void RectMode(DrawMode mode);

		/// <summary>
		/// Draws a rectangle based on the parameters
		/// <remarks>see documentation RectMode</remarks>
		/// </summary>
		/// <param name="x1">x1 coordinate in pixel space. This parameter is varying based on the RectMode</param>
		/// <param name="y1">y1 coordinate in pixel space. This parameter is varying based on the RectMode</param>
		/// <param name="x2">x2 coordinate in pixel space. This parameter is varying based on the RectMode</param>
		/// <param name="y2">y2 coordinate in pixel space. This parameter is varying based on the RectMode</param>
		/// <param name="cornerRadiusX">the corner radius for left & right side of the rectangle</param>
		/// <param name="cornerRadiusY">the corner radius for top & bottom side of the rectangle</param>
		void Rect(float x1, float y1, float x2, float y2, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);

		/// <summary>
		/// Calls Rect() with width and height being the size
		/// parameter.
		/// 
		/// <remarks>seed documentation RectMode</remarks>
		/// </summary>
		/// <param name="x1">x1 coordinate in pixel space. This parameter is varying based on the RectMode</param>
		/// <param name="y1">y1 coordinate in pixel space. This parameter is varying based on the RectMode</param>
		/// <param name="size">the size of the Quad. This parameter is varying based on the RectMode</param>
		/// <param name="cornerRadiusX">the corner radius for left & right side of the rectangle</param>
		/// <param name="cornerRadiusY">the corner radius for top & bottom side of the rectangle</param>
		void Quad(float x1, float y1, float size, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);

		/// <summary>
		/// Changes the drawmode for ellipsis
		/// 
		/// <remarks>see documentation DrawMode</remarks>
		/// </summary>
		/// <param name="mode">the drawmode to use</param>
		void EllipseMode(DrawMode mode);

		/// <summary>
		/// draws an ellipse based on the current
		/// drawmode for ellipsis
		/// </summary>
		/// <param name="x">x position in pixel space. This parameter is varying based on the EllipseMode</param>
		/// <param name="y">y position in pixel space. This parameter is varying based on the EllipseMode</param>
		/// <param name="rx">radius-x in pixel space. This parameter is varying based on the EllipseMode</param>
		/// <param name="ry">radius-y in pixel space. This parameter is varying based on the EllipseMode</param>
		void Ellipse(float x, float y, float rx, float ry);

		/* draws an ellipsed with the same radii's */
		/// <summary>
		/// Calls Ellipse() with the radius being
		/// rx and ry
		/// </summary>
		/// <param name="x">x position in pixel space. This parameter is varying based on the EllipseMode</param>
		/// <param name="y">y position in pixel space. This parameter is varying based on the EllipseMode</param>
		/// <param name="radius">radius in pixel space. This parameter is varying based on the EllipseMode</param>
		void Circle(float x, float y, float radius);

		/// <summary>
		/// Draws a point to the screen.
		/// It uses the stroke as fill and
		/// StrokeWeight as radius
		/// </summary>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		void Point(float x, float y);

		/* draws a line from x1, y1 to x2, y2. It uses the stroke as fill */
		/// <summary>
		/// Draws a line from x1, y1 to x2, y2.
		/// It uses the stroke as fill and StrokeWeight
		/// as line width
		/// </summary>
		/// <param name="x1">start x in pixel space</param>
		/// <param name="y1">start y in pixel space</param>
		/// <param name="x2">end x in pixel space</param>
		/// <param name="y2">end y in pixel space</param>
		void Line(float x1, float y1, float x2, float y2);

	private:

		/// <summary>
		/// returns the current shape of the
		/// active RenderState
		/// </summary>
		RenderState & GetState();

	private:

		/// <summary>
		/// An instance of the Renderer
		/// to draw the shape
		/// </summary>
		Renderer *& renderer;

		/// <summary>
		/// An instance of the RenderStateManager
		/// to get access to the brushes / drawing
		/// properties
		/// </summary>
		RenderStateManager *& rsm;

	};

	/// <summary>
	/// Example:
	/// <code>
	/// 
	/// ...
	/// StrokeWeight(2.f);
	/// Stroke(Core::Color::LightPink);
	/// 
	/// Fill(Core::Color::LightRed);
	/// Rect(100.f, 100.f, 75.f, 30.f);
	/// 
	/// Fill(Core::Color::LightBlue);
	/// Quad(200.f, 100f.f, 50.f);
	/// 
	/// Fill(Core::Color::LightYellow);
	/// Ellipse(100.f, 200.f, 15.f);
	/// 
	/// Fill(Core::Color::LightOrange);
	/// StrokeWeight(4.f);
	/// Point(50.f, 50.f);
	/// ...
	/// 
	/// </code>
	/// </summary>

}