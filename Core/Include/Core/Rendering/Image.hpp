#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/System/Collection.hpp>

#include <Core/Rendering/Color.hpp>

#include <string>

struct ID2D1Bitmap;

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
		Linear,
	};

	class GraphicsContext;

	/*class Image {
	public:

		enum FileType {
			Unknown,
			PNG,
			BMP,
			TGA,
			JPG
		};

		Image();
		
		bool create(i32_t width, i32_t height, const Color & shade, i32_t channels, i32_t opacity, ImageInterpolationMode mode, GraphicsContext * gctx);

		bool updatePixels();

		bool loadFromFile(const std::string & filepath, GraphicsContext * gctx);
		bool loadFromImage(const Image & source, i32_t x, i32_t y, i32_t width, i32_t height);

		bool saveToFile(const std::string & filepath);

		u32_t getIndex(i32_t x, i32_t y) const;
		Color getColor(i32_t x, i32_t y) const;
		void setColor(i32_t x, i32_t y, Color color);

		ID2D1Bitmap * getBitmap() const;

		static FileType getFileType(const std::string & filepath);

	private:

		bool createBitmap(const Color * data);

	public:

		i32_t width;
		i32_t height;
		i32_t opacity;
		i32_t channels;

		ImageInterpolationMode mode;

		Vector<u8_t> pixels;

	private:

		struct Implementation;
		std::shared_ptr<Implementation> impl;
	
		GraphicsContext * gctx;

	};*/

}