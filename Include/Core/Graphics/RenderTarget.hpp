// 
// RenderTarget.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Graphics/RenderStyle.hpp>
#include <Core/Graphics/Shape.hpp>
#include <Core/Graphics/Texture.hpp>

#include <Core/System/Rectangle.hpp>

#include <stack>

////////////////////////////////////////////////////////////
/// Forward declaration
/// 
////////////////////////////////////////////////////////////
struct ID2D1RenderTarget;

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define base class for rendering commands.
	///
	/// This abstract class contains all functionality for
	/// rendering content to the render target.
	///
	////////////////////////////////////////////////////////////
	class RenderTarget
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////
		RenderTarget();

		////////////////////////////////////////////////////////////
		/// \brief Default destructor.
		/// 
		////////////////////////////////////////////////////////////
		virtual ~RenderTarget() = default;

		////////////////////////////////////////////////////////////
		/// \brief Clears the background to the given color.
		///
		///	\param color The color to paint the background in.
		/// 
		////////////////////////////////////////////////////////////
		void Background(const Color& color);

		////////////////////////////////////////////////////////////
		/// \brief Sets the fill color to match the given color
		///		   and activates the solid fill brush.
		///
		///	\param color The color to use for filling objects.
		/// 
		////////////////////////////////////////////////////////////
		void Fill(const Color& color);

		////////////////////////////////////////////////////////////
		/// \brief Disables the fill brush.
		/// 
		////////////////////////////////////////////////////////////
		void NoFill();

		////////////////////////////////////////////////////////////
		/// \brief Sets the fill color to match the given color
		///		   and activates the solid stroke brush.
		///
		///	\param color The color to use for outlining objects.
		/// 
		////////////////////////////////////////////////////////////
		void Stroke(const Color& color);

		////////////////////////////////////////////////////////////
		/// \brief Disables the outline brush.
		/// 
		////////////////////////////////////////////////////////////
		void NoStroke();

		////////////////////////////////////////////////////////////
		/// \brief Set the outline thickness.
		///
		///	\param weight The new outline thickness.
		/// 
		////////////////////////////////////////////////////////////
		void StrokeWeight(float weight);

		////////////////////////////////////////////////////////////
		/// \brief Set the draw mode used for rectangles.
		/// 
		////////////////////////////////////////////////////////////
		void RectMode(DrawMode mode);

		////////////////////////////////////////////////////////////
		/// \brief Draws a rectangle on screen.
		///
		///	The parameters are interpreted based on the rect mode.
		///
		///	\param x1		X-coordinate of the rectangle by default.
		///	\param y1		Y-coordinate of the rectangle by default.
		///	\param x2		Width of the rectangle by default.
		///	\param y2		Height of the rectangle by default.
		///	\param cornerX	Horizontal corner radius
		///	\param cornerY	Vertical corner radius
		/// 
		////////////////////////////////////////////////////////////
		void Rect(float x1, float y1, float x2, float y2, float cornerX, float cornerY);

		////////////////////////////////////////////////////////////
		/// \brief Set the draw mode used for ellipsis.
		/// 
		////////////////////////////////////////////////////////////
		void EllipseMode(DrawMode mode);

		////////////////////////////////////////////////////////////
		/// \brief Draws an ellipse (oval) to the screen.
		///
		///	An ellipse with equal with and height is a circle. By
		///	default, the first two parameters set the location, and
		///	the third and fourth parameters set the shape's width
		///	and height. The origin may changed with the EllipseMode()
		///	method.
		/// 
		////////////////////////////////////////////////////////////
		void Ellipse(float a, float b, float c, float d);

		////////////////////////////////////////////////////////////
		/// \brief Draw a line from x1, y1 to x2, y2
		/// 
		////////////////////////////////////////////////////////////
		void Line(float x1, float y1, float x2, float y2);

		void ShapeFillMode(Shape::FillMode mode);
		void ShapeSegmentFlags(Shape::PathSegment flags);
		void BeginShape();
		void AddVertex(float x, float y);
		void AddVertex(const Float2& point);
		void AddBezier(float x1, float y1, float x2, float y2, float x3, float y3);
		void AddBezier(const Float2& start, const Float2& center, const Float2& end);
		void AddQuadraticBezier(float x1, float y1, float x2, float y2);
		void AddQuadraticBezier(const Float2& start, const Float2& end);
		void EndShape(ShapeEnd style);

		void PushTransform(bool advance);
		void PopTransform();
		void ResetTransform();
		void Translate(float x, float y);
		void Rotate(const Angle& rotation);
		void Scale(float factorX, float factorY);
		const Transformation& GetTransform() const;
		Transformation& GetTransform();
		
		void PushStyle();
		void PopStyle();

		void ImageOpacity(u8 opacity);
		void ImageSampleMode(Texture::SampleMode sampleMode);
		void ImageMode(DrawMode mode);
		void Image(const Texture& texture, float a, float b);
		void Image(const Texture& texture, float a, float b, float c, float d);
		void Image(const Texture& texture, float a, float b, float c, float d, const FloatRect& sourceRectangle);

		////////////////////////////////////////////////////////////
		/// \brief Render a shape object on screen.
		/// 
		////////////////////////////////////////////////////////////
		void Geometry(const Shape& shape);

		////////////////////////////////////////////////////////////
		/// \brief Get the current rendering style.
		/// 
		////////////////////////////////////////////////////////////
		const RenderStyle& GetRenderStyle() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the current rendering style.
		/// 
		////////////////////////////////////////////////////////////
		RenderStyle& GetRenderStyle();

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying shape object as pure reference.
		/// 
		////////////////////////////////////////////////////////////
		Shape& GetGeometry();

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying shape object as const reference.
		/// 
		////////////////////////////////////////////////////////////
		const Shape& GetGeometry() const;
		
		////////////////////////////////////////////////////////////
		/// \brief Abstract method to receive a Direct2D render
		///		   target.
		///
		///	All rendering commands are redirected to the underlying
		///	Direct2D RenderTarget.
		/// 
		////////////////////////////////////////////////////////////
		virtual ID2D1RenderTarget& GetRenderTarget() = 0;

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::stack<RenderStyle> styles;		///< The rendering styles
		Shape					geometry;	///< Geometry to build and render

	};

}