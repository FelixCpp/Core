#pragma once

namespace Core
{

    /// <summary>
    /// Enum which describes how to sample from a source outside its base tile.
    /// </summary>
    enum class ExtendMode {

        /// <summary>
        /// Extend the edges of the source out by clamping sample points outside the source
        /// to the edges.
        /// </summary>
        Clamp,

        /// <summary>
        /// The base tile is drawn untransformed and the remainder are filled by repeating
        /// the base tile.
        /// </summary>
        Wrap,

        /// <summary>
        /// The same as wrap, but alternate tiles are flipped  The base tile is drawn
        /// untransformed.
        /// </summary>
        Mirror,
    };

}