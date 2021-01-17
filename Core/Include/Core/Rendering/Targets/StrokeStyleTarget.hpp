#pragma once

#include <vector>

#include <Core/Rendering/StrokeStyleAttributes.hpp>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class StrokeStyle;
	class RenderStateManager;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// strokes/outlines
	/// </summary>
	class StrokeStyleTarget {
	public:

		/// <summary>
		/// constructor which passes the RenderStateManager
		/// to its member variable.
		/// </summary>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState holding the strokestyle attribute</param>
		explicit StrokeStyleTarget(RenderStateManager *& rsm);

		/// <summary>
		/// A virtual destructor
		/// </summary>
		virtual ~StrokeStyleTarget() = default;

		/// <summary>
		/// changes the style the line
		/// starts
		/// </summary>
		/// <param name="style">the style</param>
		void StrokeStartCap(CapStyle style);

		/// <summary>
		/// changes the style the line
		/// ends
		/// </summary>
		/// <param name="style">the style</param>
		void StrokeEndCap(CapStyle style);

		/// <summary>
		/// changes the style the line
		/// dashes are connected with 
		/// each other
		/// </summary>
		/// <param name="style">the style</param>
		void StrokeDashCap(CapStyle style);

		/// <summary>
		/// changes the style the line
		/// joins together
		/// </summary>
		/// <param name="lineJoin">the style</param>
		void StrokeLineJoin(LineJoin lineJoin);

		/// <summary>
		/// changes the dash style of the line
		/// </summary>
		/// <param name="style">the style</param>
		void StrokeDashStyle(DashStyle style);

		/// <summary>
		/// defines the dash pattern of the line
		/// </summary>
		/// <param name="dashes"></param>
		void StrokeDashes(const std::vector<float> & dashes);

		/// <summary>
		/// changes the offset between the dashes
		/// </summary>
		/// <param name="offset">the offset in pixels</param>
		void StrokeDashOffset(float offset);

		/// <summary>
		/// changes the limit of the dashes miter
		/// </summary>
		/// <param name="limit">the limit</param>
		void StrokeMiterLimit(float limit);

	private:

		/// <summary>
		/// returns the current stroke style of the
		/// active RenderState
		/// </summary>
		StrokeStyle & GetStyle();

	private:

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
	/// StrokeStartCap(Core::CapStyle::Round);
	/// StrokeEndCap(Core::CapStyle::Triangle);
	/// StrokeDashCap(Core::CapStyle::Flat);
	/// 
	/// StrokeWeight(4.f);
	/// Stroke(Core::Color::LightGreen);
	/// Line(100.f, 100.f, mouseX, mouseY);
	/// ...
	/// 
	/// </code>
	/// </summary>

}