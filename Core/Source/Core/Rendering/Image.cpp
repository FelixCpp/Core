#include <Core/Rendering/Image.hpp>

// GraphicsContext (includes d2d1.h / wrl.h)
#include <Core/Rendering/GraphicsContext.hpp>

// contains helper functions to get the red / green and blue component from a color
#include <Core/Rendering/Helpers.hpp>

// constrain
#include <Core/Maths/Math.hpp>

// for logging errors
#include <Core/System/Logger.hpp>

// loading images with stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

// saving images with stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

namespace Core
{

	//struct Image::Implementation {

	//	Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap;

	//	Implementation() :
	//		bitmap(nullptr)
	//	{ }

	//};

	//Image::Image() :
	//	width(0), height(0), opacity(255), channels(0), mode(ImageInterpolationMode::NearestNeighbor), pixels(),
	//	impl(std::make_shared<Implementation>()), gctx(nullptr)
	//{
	//}

	//bool Image::create(i32_t width, i32_t height, const Color & shade, i32_t channels, i32_t opacity, ImageInterpolationMode mode, GraphicsContext * gctx)
	//{
	//	// copy attributes
	//	this->width = width;
	//	this->height = height;
	//	this->channels = channels;
	//	this->opacity = opacity;
	//	this->mode = mode;
	//	this->gctx = gctx;

	//	// get the components from the shade parameter
	//	const i32_t r = red(shade);
	//	const i32_t g = green(shade);
	//	const i32_t b = blue(shade);
	//	const i32_t a = channels > 3 ? alpha(shade) : 255;

	//	// create space for the pixels and initialize them to black
	//	this->pixels.resize(width * height * channels, 0x00);

	//	// copy the shade into the pixels
	//	for (u32_t i = 0; i < width * height * channels; i += channels)
	//	{
	//		this->pixels[i + 0] = r;
	//		this->pixels[i + 1] = g;
	//		this->pixels[i + 2] = b;
	//		this->pixels[i + 3] = a;
	//	}

	//	// create bitmap
	//	return this->updatePixels();
	//}

	//bool Image::updatePixels()
	//{
	//	// map the data to argb-format
	//	color_t * data = new color_t[this->width * this->height]{ 0xFF000000 };
	//	for (color_t i = 0, n = 0; i < this->width * this->height * this->channels; i += this->channels, n++)
	//	{
	//		const u8_t & r = this->pixels[i + 0];
	//		const u8_t & g = this->pixels[i + 1];
	//		const u8_t & b = this->pixels[i + 2];
	//		const u8_t   a = this->channels > 3 ? this->pixels[i + 3] : 255;
	//		data[n] = color(r, g, b, a);
	//	}

	//	// store the result to order to return it later
	//	const bool success = this->createBitmap(data);
	//	
	//	// delete the data
	//	if (data)
	//	{
	//		delete[] data;
	//		data = nullptr;
	//	}

	//	return success;
	//}

	//bool Image::loadFromFile(const std::string & filepath, GraphicsContext * gctx)
	//{
	//	// load the pixel data from the file
	//	stbi_uc * pixelData = stbi_load(filepath.c_str(), &this->width, &this->height, &this->channels, 0);
	//	
	//	// error handling
	//	if (pixelData == NULL)
	//	{
	//		// an error occurred
	//		CORE_ERROR("Failed to load \"%s\"", filepath.c_str());
	//		return false;
	//	}

	//	// loading was successful
	//	this->pixels.resize(this->width * this->height * this->channels);
	//	this->pixels.assign(pixelData, pixelData + this->pixels.size());

	//	// delete the memory
	//	stbi_image_free(pixelData);

	//	// important: set the graphicscontext before recreating the bitmap
	//	this->gctx = gctx;
	//	
	//	return this->updatePixels();
	//}

	//bool Image::saveToFile(const std::string & filepath)
	//{
	//	const FileType fileType = Image::getFileType(filepath);

	//	// copy attributes
	//	const i32_t & x = this->width;
	//	const i32_t & y = this->height;
	//	const i32_t & comp = this->channels;
	//	const u8_t * data = this->pixels.data();
	//	const char * filename = filepath.c_str();

	//	i32_t success = FALSE;
	//	switch (fileType)
	//	{
	//			// save PNG
	//		case PNG: success = stbi_write_png(filename, x, y, comp, data, x * comp);  break;
	//		
	//			// save BMP
	//		case BMP: success = stbi_write_bmp(filename, x, y, comp, data); break;
	//		
	//			// save TGA
	//		case TGA: success = stbi_write_tga(filename, x, y, comp, data); break;
	//		
	//			// save JPG
	//		case JPG: success = stbi_write_jpg(filename, x, y, comp, data, 100); break;
	//		
	//			// invalid Filetype
	//		default: success = FALSE; break;
	//	}

	//	// error handling
	//	if (success == FALSE)
	//	{
	//		// an error occurred
	//		CORE_ERROR("Failed to save data to \"%s\"", filepath.c_str());
	//		return false;
	//	}

	//	return true;
	//}

	//u32_t Image::getIndex(i32_t x, i32_t y) const
	//{
	//	return (y * this->width + x) * this->channels;
	//}

	//color_t Image::getColor(i32_t x, i32_t y) const
	//{
	//	const u32_t index = this->getIndex(x, y);

	//	// create a color from the selected pixel
	//	return color(
	//		this->pixels[index + 0],
	//		this->pixels[index + 1],
	//		this->pixels[index + 2],
	//		this->channels > 3 ? this->pixels[index + 3] : 255
	//	);
	//}

	//void Image::setColor(i32_t x, i32_t y, color_t color)
	//{
	//	const u32_t index = this->getIndex(x, y);

	//	// bounds-check
	//	if (index >= 0 && index < this->width * this->height * this->channels)
	//	{
	//		// change the r, g, b and alpha value of the existing color
	//		// at the position
	//		this->pixels[index + 0] = red(color);
	//		this->pixels[index + 1] = green(color);
	//		this->pixels[index + 2] = blue(color);

	//		if (this->channels > 3)
	//			this->pixels[index + 3] = alpha(color);
	//	}
	//}

	//bool Image::loadFromImage(const Image & source, i32_t x, i32_t y, i32_t width, i32_t height)
	//{
	//	// calculate the edges
	//	i32_t left   = (i32_t)std::fmax(x, 0);
	//	i32_t top    = (i32_t)std::fmax(y, 0);
	//	i32_t right  = (i32_t)Math::constrain(left + width, 0, source.width);
	//	i32_t bottom = (i32_t)Math::constrain(top + height, 0, source.height);

	//	// copy attributes
	//	this->width = (right - left);
	//	this->height = (bottom - top);
	//	this->channels = source.channels;
	//	this->gctx = source.gctx;
	//	
	//	// calculate the size
	//	const u32_t size = width * height * this->channels;
	//	
	//	// we don't need to create a bitmap if the size is zero.
	//	if (size == 0)
	//	{
	//		CORE_ERROR("There is no data to copy from");
	//		return false;
	//	}

	//	// resize the vector of pixels
	//	this->pixels.resize(size, 0x00);

	//	// go through the area and copy the pixel
	//	// at that location into the new image
	//	for (i32_t y = top, ys = 0; y < bottom; y++, ys++)
	//	{
	//		for (i32_t x = left, xs = 0; x < right; x++, xs++)
	//		{
	//			const color_t & color = source.getColor(x, y);
	//			this->setColor(xs, ys, color);
	//		}
	//	}

	//	// create the bitmap with the new pixels and return the result
	//	return this->updatePixels();
	//}

	//ID2D1Bitmap * Image::getBitmap() const
	//{
	//	return this->impl->bitmap.Get();
	//}

	//Image::FileType Image::getFileType(const std::string & filepath)
	//{
	//	const std::size_t dotPos = filepath.find_last_of('.'); // get the position of the last '.' character
	//	std::string extension = filepath.substr(dotPos); // get the extension including the dot (example: .png / .jpg)
	//	std::transform(extension.begin(), extension.end(), &extension[0], std::tolower); // make the string lowercased
	//
	//	if (extension == ".png") return PNG;
	//	if (extension == ".bmp") return BMP;
	//	if (extension == ".tga") return TGA;
	//	if (extension == ".jpg") return JPG;
	//	
	//	return Unknown;
	//}

	//bool Image::createBitmap(const color_t * data)
	//{
	//	if (!this->gctx)
	//	{
	//		CORE_ERROR("The image has no GraphicsContext");
	//		return false;
	//	}

	//	// get the rendertarget
	//	ID2D1HwndRenderTarget * renderTarget = this->gctx->hwndRenderTarget.Get();
	//
	//	// -- Fields to create the bitmap properties --
	//	const D2D1_PIXEL_FORMAT pixelFormat = renderTarget->GetPixelFormat();
	//	
	//	FLOAT dpiX = 0.f, dpiY = 0.f;
	//	renderTarget->GetDpi(&dpiX, &dpiY);
	//	// --
	//
	//	const void * srcData = data; // get the pixels data
	//	const UINT32 pitch = this->width * sizeof color_t + 0; // calculate pitch (width * bytesize (color_t) + byteoffset in structure)
	//
	//	const HRESULT hr = renderTarget->CreateBitmap(
	//		D2D1::SizeU(this->width, this->height),
	//		srcData,
	//		pitch,
	//		D2D1::BitmapProperties(pixelFormat, dpiX, dpiY),
	//		&this->impl->bitmap
	//	);
	//
	//	// error handling
	//	if (FAILED(hr))
	//	{
	//		// an error occurred
	//		CORE_ERROR("Failed to create a bitmap");
	//		return false;
	//	}
	//
	//	return true;
	//}

}