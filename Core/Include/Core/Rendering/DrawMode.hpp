#pragma once

namespace Core
{

	enum class DrawMode {
		Corner,
		Corners,
		Center
	};

	/*
	
		example (rectangle / image):
			drawRect(x1, y1, x2, y2)

			DrawMode::Corner:
				treats x1, y1 as the starting x, y location
				treats x2, y2 as the width and height of the rectangle
				
			DrawMode::Corners:
				treats x1, y1 as the starting x, y location
				treats x2, y2 as the end x, y location

			DrawMode::Center:
				teats x1, y1 as the center of the rectangle
				treats x2, y2 as the width and height of the rectangle

		------------------------------------------------------------------------------------------

		example (circle):
			drawRect(x, y, radiusX, radiusY)

			DrawMode::Corner:
				treats x, y as the starting x, y location
				treats radiusX, radiusY as the radii of the circle

			DrawMode::Corners:
				treats x, y as the starting x, y location
				treats radiusX, radiusY as the end x, y location

			DrawMode::Center:
				teats x, y as the center of the rectangle
				treats radiusX, radiusY as the radii of the circle

	*/

}