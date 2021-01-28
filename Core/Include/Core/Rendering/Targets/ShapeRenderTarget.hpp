#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/ShapeAttributes.hpp>
#include <Core/System/LateRef.hpp>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class Shape;
	class RenderStateManager;
	class Renderer;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// geometries/shapes
	/// </summary>
	class ShapeRenderTarget {
	public:

		/// <summary>
		/// constructor which passes the Renderer & RenderStateManager
		/// to its member variables.
		/// 
		/// Its needed to get access to the Shape attribute
		/// in the active RenderState
		/// </summary>
		/// <param name="renderer">the Renderer which is needed to draw the shape</param>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState which holds the shape object</param>
		explicit ShapeRenderTarget(LateRef<Renderer> renderer, LateRef<RenderStateManager> rsm);

		/// <summary>
		/// A virtual destructor
		/// </summary>
		virtual ~ShapeRenderTarget() = default;

		/// <summary>
		/// Tells the shape to prepare getting
		/// geometry calls like:
		///		Vertex,
		///		Bezier,
		///		QuadraticBezier,
		///		Arc
		/// 
		/// This method must be called before calling
		/// any of the listed functions above
		/// </summary>
		/// <param name="begin">defines the style the shape begins</param>
		void BeginShape(ShapeBegin begin = ShapeBegin::Filled);

		/// <summary>
		/// Tells the shape to stop listening
		/// on geometry calls and calls DrawShape()
		/// 
		/// <remarks>maybe take a look at BeginShape() documentation</remarks>
		/// </summary>
		/// <param name="end">defines the style the shape ends</param>
		void EndShape(ShapeEnd end = ShapeEnd::Open);

		/// <summary>
		/// changes the shapes fill-mode
		/// 
		/// This method can't be called
		/// between BeginShape() and
		/// EndShape()
		/// </summary>
		/// <param name="fillMode">type of fillmode</param>
		void ShapeFillMode(FillMode fillMode);

		/// <summary>
		/// changes the shapes segment path
		/// 
		/// This method can't be called
		/// between BeginShape() and
		/// EndShape()
		/// </summary>
		/// <param name="pathSegment">type of segment path</param>
		void ShapePathSegment(PathSegment pathSegment);

		/// <summary>
		/// Adds a point into the geometry/shape
		/// The points in a shape gets connected
		/// through lines.
		/// 
		/// This call can be executed if
		/// BeginShape() was called before.
		/// </summary>
		/// <param name="x">point x in pixel space</param>
		/// <param name="y">point y in pixel space</param>
		void Vertex(float x, float y);

		/// <summary>
		/// Adds a quadratic bezier segment into
		/// the geometry/shape structure.
		/// 
		/// This call can be executed if
		/// BeginShape() was called before.
		/// </summary>
		/// <param name="x1">start point x in pixel space</param>
		/// <param name="y1">start point y in pixel space</param>
		/// <param name="x2">end point x in pixel space</param>
		/// <param name="y2">end point y in pixel space</param>
		void QuadraticBezier(float x1, float y1, float x2, float y2);

		/// <summary>
		/// Adds a bezier segment into
		/// the geometry/shape structure.
		/// 
		/// This call can be executed if
		/// BeginShape() was called before
		/// </summary>
		/// <param name="x1">start point x in pixel space</param>
		/// <param name="y1">start point y in pixel space</param>
		/// <param name="x2">center point x in pixel space</param>
		/// <param name="y2">center point x in pixel space</param>
		/// <param name="x3">end point x in pixel space</param>
		/// <param name="y3">end point x in pixel space</param>
		void Bezier(float x1, float y1, float x2, float y2, float x3, float y3);

		/// <summary>
		/// Adds an arc into the
		/// geometry/shape structure.
		/// </summary>
		/// <param name="x">point x in pixel space</param>
		/// <param name="y">point y in pixel space</param>
		/// <param name="width">width of the arc in pixels</param>
		/// <param name="height">height of the arc in pixels</param>
		/// <param name="rotation">rotation in degrees</param>
		/// <param name="direction">direction in which the arc faces</param>
		/// <param name="size">the size-type of the arc</param>
		void Arc(float x, float y, float width, float height, float rotation, SweepDirection direction, ArcSize size);

	private:
		
		/// <summary>
		/// returns the current shape of the
		/// active RenderState
		/// </summary>
		Shape & GetShape();

		/// <summary>
		/// Draws the shape build by
		/// the commands Vertex, Bezier etc.
		/// to the screen
		/// </summary>
		void DrawShape();

	private:

		/// <summary>
		/// An instance of the Renderer
		/// to draw the shape
		/// </summary>
		LateRef<Renderer> renderer;

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
	/// BeginShape();
	/// Vertex(300.f, 300.f);
	/// Vertex(300.f, 600.f);
	/// Vertex(600.f, 600.f);
	/// Vertex(600.f, 300.f);
	/// Bezier(600.f, 300.f, 450.f, 100.f, 300.f, 300.f);
	/// EndShape();
	/// ...
	/// 
	/// </code>
	/// </summary>
	
}