#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Image.hpp> // Core::Image
#include <Core/Rendering/DrawMode.hpp> // Core::DrawMode
#include <Core/System/LateRef.hpp> // Core::LateRef

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class RenderStateManager;
	class Renderer;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// images
	/// </summary>
	class ImageRenderTarget {
	public:

		/// <summary>
		/// constructor which passes the Renderer & RenderStateManager
		/// to its member variables.
		/// </summary>
		/// <param name="renderer">the Renderer which is needed to draw the image</param>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState which holds the current image mode</param>
		explicit ImageRenderTarget(LateRef<Renderer> gctx, LateRef<RenderStateManager> rsm);

		/// <summary>
		/// virtual destructor
		/// </summary>
		virtual ~ImageRenderTarget() = default;

		/// <summary>
		/// Creates a flat image based on
		/// the parameters
		/// </summary>
		/// <param name="width">width of the image in pixels</param>
		/// <param name="height">height of the image in pixels</param>
		/// <param name="color">a flat color copied by each pixel</param>
		Image CreateImage(u32_t width, u32_t height, const Color & color = Color::White);

		/// <summary>
		/// Creates an image based on
		/// the parameters
		/// </summary>
		/// <param name="width">the width of the image in pixels</param>
		/// <param name="height">the height of the image in pixels</param>
		/// <param name="colors">an array[width * height] holding a color value for each pixel</param>
		Image LoadImageFromMemory(u32_t width, u32_t height, const Color * colors);

		/// <summary>
		/// Loads an image from the filepath
		/// </summary>
		/// <param name="filepath">a path starting from your root directory</param>
		Image LoadImageFromFile(const std::string & filepath);

		/// <summary>
		/// Copies an area from the window
		/// </summary>
		/// <param name="x">start x position in pixel space</param>
		/// <param name="y">start y position in pixel space</param>
		/// <param name="width">width of the image in pixels</param>
		/// <param name="height">height of the image in pixels</param>
		Image LoadImageFromScreen(i32_t x, i32_t y, i32_t width, i32_t height);

		/// <summary>
		/// Copies a section from the source
		/// </summary>
		/// <param name="source">the image to copy from</param>
		/// <param name="x">start x position in pixels</param>
		/// <param name="y">start y position in pixels</param>
		/// <param name="width">width of the area to copy</param>
		/// <param name="height">height of the area to copy</param>
		/// <returns>A new image containing the copied content</returns>
		Image LoadImageFromImage(const Core::Image & source, i32_t x, i32_t y, i32_t width, i32_t height);

		/// <summary>
		/// Changes the drawmode of
		/// images for the current RenderState
		/// </summary>
		/// <param name="mode">the new draw-mode</param>
		void ImageMode(DrawMode mode);

		/// <summary>
		/// Draws an image at x, y with the images width and height as size
		/// </summary>
		/// <param name="image">the image to draw</param>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		void Image(const Core::Image & image, float x, float y);

		/// <summary>
		/// Draws an image at x, y with the specified width and height as size
		/// </summary>
		/// <param name="image">the image to draw</param>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		/// <param name="width">width of the image in pixels. The content gets stretched to fit everything from x to x + width</param>
		/// <param name="height">height of the image in pixels. The content gets stretched to fit everything from y to y + height</param>
		void Image(const Core::Image & image, float x, float y, float width, float height);

	private:

		/// <summary>
		/// An instance of the Renderer.
		/// This attribute is used to load/create
		/// and draw images
		/// </summary>
		LateRef<Renderer> renderer;

		/// <summary>
		/// An instance of the RenderStateManager.
		/// This attribute is used to draw
		/// the image correctly based on the
		/// ImageMode from the active RenderState
		/// </summary>
		LateRef<RenderStateManager> rsm;

	}; // class ImageRenderTarget

	/// <summary>
	/// Example:
	/// <code>
	/// 
	/// Image myImage = CreateImage(400, 400, Core::Color::LightRed);
	/// 
	/// ...
	/// Image(myImage, 0.f, 0.f, mouseX, mouseY);
	/// ...
	/// 
	/// </code>
	/// </summary>

} // namespace Core