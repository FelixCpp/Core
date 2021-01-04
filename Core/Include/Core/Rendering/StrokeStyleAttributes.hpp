#pragma once

namespace Core
{

    /// <summary>
    /// Enum which describes the drawing of the ends of a line.
    /// </summary>
    enum class CapStyle {

        /// <summary>
        /// Flat line cap.
        /// </summary>
        Flat,

        /// <summary>
        /// Square line cap.
        /// </summary>
        Square,

        /// <summary>
        /// Round line cap.
        /// </summary>
        Round,

        /// <summary>
        /// Triangle line cap.
        /// </summary>
        Triangle,

    };


    /// <summary>
    /// Describes the sequence of dashes and gaps in a stroke.
    /// </summary>
    enum class DashStyle {
        Solid,
        Dash,
        Dot,
        DashDot,
        DashDotDot,
        Custom,
    };


    /// <summary>
    /// Enum which describes the drawing of the corners on the line.
    /// </summary>
    enum class LineJoin {

        /// <summary>
        /// Miter join.
        /// </summary>
        Miter,

        /// <summary>
        /// Bevel join.
        /// </summary>
        Bevel,

        /// <summary>
        /// Round join.
        /// </summary>
        Round,

        /// <summary>
        /// Miter/Bevel join.
        /// </summary>
        MiterOrBevel,
    };

}