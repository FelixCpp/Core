#pragma once

namespace Core
{

	enum class ShapeEndType {
		Open, // leave the shape as it is
		Closed // connect the end of the shape with the starting point
	};

	enum class FillMode {
		Alternate,
		Winding
	};

	enum class PathSegment {
		None,
		Unstorked,
		ForceRoundLineJoin
	};

	/// <summary>
	/// Defines the direction that an elliptical arc is drawn.
	/// </summary>
	enum class SweepDirection {
		CounterClockwise = 0,
		Clockwise = 1,
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