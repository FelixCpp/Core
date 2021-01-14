#pragma once

namespace Core
{

	enum class ShapeEnd {
		Open, // leave the shape as it is
		Closed // connect the end of the shape with the starting point
	};

	/// <summary>
	/// Indicates whether the given figure is filled or hollow.
	/// </summary>
	enum class ShapeBegin {
		Filled,
		Hollow,
	};

	/// <summary>
	/// Specifies how the intersecting areas of geometries or figures are combined to
	/// form the area of the composite geometry.
	/// </summary>
	enum class FillMode {
		Alternate,
		Winding,
	};

	/// <summary>
	/// Indicates whether the given segment should be stroked, or, if the join between
	/// this segment and the previous one should be smooth.
	/// </summary>
	enum class PathSegment {
		None,
		Unstroked,
		RoundLineJoin,
	};

	/// <summary>
	/// Defines the direction that an elliptical arc is drawn.
	/// </summary>
	enum class SweepDirection {
		CounterClockwise,
		Clockwise,
	};

	/// <summary>
	/// Differentiates which of the two possible arcs could match the given arc
	/// parameters.
	/// </summary>
	enum class ArcSize {
		Small,
		Large,
	};

}