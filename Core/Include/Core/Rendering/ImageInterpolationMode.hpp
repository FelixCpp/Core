#pragma once

namespace Core
{

	/// <summary>
	/// Specifies the algorithm that is used when images are scaled or rotated. Note
	/// Starting in Windows 8, more interpolations modes are available. See
	/// D2D1_INTERPOLATION_MODE for more info.
	/// </summary>
	enum class ImageInterpolationMode {
		/// <summary>
		/// Nearest Neighbor filtering. Also known as nearest pixel or nearest point
		/// sampling.
		/// </summary>
		NearestNeighbor,

		/// <summary>
		/// Linear filtering.
		/// </summary>
		Linear
	};

}