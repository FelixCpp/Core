// 
// RenderTarget.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/RenderTarget.hpp>
#include <Core/Graphics/Shape.hpp>
#include <Core/Graphics/Texture.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d2d1.h>

namespace Core
{

	////////////////////////////////////////////////////////////
	RenderTarget::RenderTarget()
	{
		// push the initial rendering style
		PushStyle();
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Background(const Color& color)
	{
		ID2D1RenderTarget& rt = GetRenderTarget();
		rt.Clear(D2D1::ColorF(
			(float)color.R / 255.0f,
			(float)color.G / 255.0f,
			(float)color.B / 255.0f,
			1.0f
		));
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Fill(const Color& color)
	{
		RenderStyle& style = GetRenderStyle();
		style.SolidFill.SetColor(color);
		style.ActiveFill = style.SolidFill.GetBrush();
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::NoFill()
	{
		GetRenderStyle().ActiveFill = nullptr;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Stroke(const Color& color)
	{
		RenderStyle& style = GetRenderStyle();
		style.SolidStroke.SetColor(color);
		style.ActiveStroke = style.SolidStroke.GetBrush();
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::NoStroke()
	{
		GetRenderStyle().ActiveStroke = nullptr;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::StrokeWeight(float weight)
	{
		GetRenderStyle().StrokeWeight = weight;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::RectMode(DrawMode mode)
	{
		GetRenderStyle().RectMode = mode;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Rect(float x1, float y1, float x2, float y2, float cornerX, float cornerY)
	{
		ID2D1RenderTarget& target = GetRenderTarget();
		const RenderStyle& style = GetRenderStyle();
		target.SetTransform(reinterpret_cast<const D2D1_MATRIX_3X2_F&>(GetTransform().GetTransform()));

		D2D1_RECT_F rect = {};
		switch(style.RectMode)
		{
			default:
			case Corner: rect = D2D1::RectF(x1, y1, x1 + x2, y1 + y2); break;
			case Corners: rect = D2D1::RectF(x1, y1, x2, y2); break;
			case Center: rect = D2D1::RectF(x1 - x2 / 2.0f, y1 - y2 / 2.0f, x1 + x2 / 2.0f, y1 + y2 / 2.0f); break;
			case Radius:
			{
				const float width = x2 * 2.0f, height = y2 * 2.0f;
				rect = D2D1::RectF(x1 - width / 2.0f, y1 - height / 2.0f, x1 + width / 2.0f, y1 + height / 2.0f);
			} break;
		}

		const D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, cornerX, cornerY);

		if(ID2D1Brush* brush = style.ActiveFill)
		{
			target.FillRoundedRectangle(roundedRect, brush);
		}

		if (ID2D1Brush* brush = style.ActiveStroke)
		{
			target.DrawRoundedRectangle(roundedRect, brush, style.StrokeWeight, style.StrokeStyle.GetStyleStroke());
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::EllipseMode(DrawMode mode)
	{
		GetRenderStyle().EllipseMode = mode;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Ellipse(float a, float b, float c, float d)
	{
		const RenderStyle& style = GetRenderStyle();
		ID2D1RenderTarget& target = GetRenderTarget();
		target.SetTransform(reinterpret_cast<const D2D1_MATRIX_3X2_F&>(GetTransform().GetTransform()));

		D2D1_ELLIPSE ellipse = {};

		switch (style.EllipseMode)
		{
			case Corner:
			{
				const float x = a, y = b, width = c, height = d;
				const float radiusX = width / 2.0f;
				const float radiusY = height / 2.0f;
				ellipse = D2D1::Ellipse(D2D1::Point2F(x + radiusX, y + radiusY), radiusX, radiusY);
			} break;

			case Corners:
			{
				const float left = a, top = b, right = c, bottom = d;
				const float radiusX = (right - left) / 2.0f;
				const float radiusY = (bottom - top) / 2.0f;
				ellipse = D2D1::Ellipse(D2D1::Point2F(left + radiusX, top + radiusY), radiusX, radiusY);
			} break;

			default:
			case Center:
			{
				const float cx = a;
				const float cy = b;
				const float radiusX = c / 2.0f;
				const float radiusY = d / 2.0f;
				ellipse = D2D1::Ellipse(D2D1::Point2F(cx, cy), radiusX, radiusY);
			} break;

			case Radius:
			{
				const float cx = a;
				const float cy = b;
				const float radiusX = c;
				const float radiusY = d;
				ellipse = D2D1::Ellipse(D2D1::Point2F(cx, cy), radiusX, radiusY);
			} break;
		}

		if (ID2D1Brush* brush = style.ActiveFill)
		{
			target.FillEllipse(ellipse, brush);
		}

		if (ID2D1Brush* brush = style.ActiveStroke)
		{
			target.DrawEllipse(ellipse, brush, style.StrokeWeight, style.StrokeStyle.GetStyleStroke());
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Line(float x1, float y1, float x2, float y2)
	{
		ID2D1RenderTarget& target = GetRenderTarget();
		const RenderStyle& style = GetRenderStyle();
		target.SetTransform(reinterpret_cast<const D2D1_MATRIX_3X2_F&>(GetTransform().GetTransform()));

		if (ID2D1Brush* brush = style.ActiveStroke)
		{
			target.DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), brush, style.StrokeWeight, style.StrokeStyle.GetStyleStroke());
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::ShapeFillMode(Shape::FillMode mode)
	{
		geometry.SetFillMode(mode);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::ShapeSegmentFlags(Shape::PathSegment flags)
	{
		geometry.SetSegmentFlags(flags);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::BeginShape()
	{
		geometry.Begin();
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::AddVertex(float x, float y)
	{
		geometry.AddVertex(x, y);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::AddVertex(const Float2& point)
	{
		geometry.AddVertex(point);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::AddBezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		geometry.AddBezier(x1, y1, x2, y2, x3, y3);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::AddBezier(const Float2& start, const Float2& center, const Float2& end)
	{
		geometry.AddBezier(start, center, end);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::AddQuadraticBezier(float x1, float y1, float x2, float y2)
	{
		geometry.AddQuadraticBezier(x1, y1, x2, y2);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::AddQuadraticBezier(const Float2& start, const Float2& end)
	{
		geometry.AddQuadraticBezier(start, end);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::EndShape(ShapeEnd style)
	{
		geometry.End(style);

		if (geometry.IsRenderable())
		{
			Geometry(geometry);
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::PushTransform(bool advance)
	{
		RenderStyle& style = GetRenderStyle();
		style.Transform.push(advance && !style.Transform.empty() ? style.Transform.top() : Transformation());
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::PopTransform()
	{
		RenderStyle& style = GetRenderStyle();
		if(style.Transform.size() > 1)
		{
			style.Transform.pop();
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::ResetTransform()
	{
		Transformation& transform = GetTransform();
		transform.SetPosition(0.0f, 0.0f);
		transform.SetRotation(Angle::Zero);
		transform.SetScale(1.0f, 1.0f);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Translate(float x, float y)
	{
		GetTransform().Move(x, y);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Rotate(const Angle& rotation)
	{
		GetTransform().Rotate(rotation);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Scale(float factorX, float factorY)
	{
		GetTransform().Scale(factorX, factorY);
	}

	////////////////////////////////////////////////////////////
	const Transformation& RenderTarget::GetTransform() const
	{
		return GetRenderStyle().Transform.top();
	}

	Transformation& RenderTarget::GetTransform()
	{
		return GetRenderStyle().Transform.top();
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::PushStyle()
	{
		styles.push(RenderStyle());
		PushTransform(false);
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::PopStyle()
	{
		if(styles.size() > 1)
		{
			styles.pop();
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::ImageOpacity(u8 opacity)
	{
		GetRenderStyle().TextureOpacity = opacity;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::ImageSampleMode(Texture::SampleMode sampleMode)
	{
		GetRenderStyle().TextureSampleMode = sampleMode;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::ImageMode(DrawMode mode)
	{
		GetRenderStyle().ImageMode = mode;
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Image(const Texture& texture, float a, float b)
	{
		const Float2& size = texture.GetSize();
		Image(texture, a, b, size.X, size.Y, FloatRect(0.0f, 0.0f, size.X, size.Y));
	}

	
	////////////////////////////////////////////////////////////
	void RenderTarget::Image(const Texture& texture, float a, float b, float c, float d)
	{
		const Float2& size = texture.GetSize();
		Image(texture, a, b, c, d, FloatRect(0.0f, 0.0f, size.X, size.Y));
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Image(
		const Texture& texture,
		float a, float b, float c, float d,
		const FloatRect& sourceRectangle
	)
	{
		// make sure there is a bitmap to render
		if(ID2D1Bitmap* bitmap = texture.GetBitmap())
		{
			ID2D1RenderTarget& target = GetRenderTarget();
			const RenderStyle& style = GetRenderStyle();

			const float x1 = a, y1 = b, x2 = c, y2 = d;
			D2D1_RECT_F destinationRectangle;
			switch(style.ImageMode)
			{
				default:
				case Corner:	destinationRectangle = D2D1::RectF(x1, y1, x1 + x2, y1 + y2); break;
				case Corners:	destinationRectangle = D2D1::RectF(x1, y1, x2, y2); break;
				case Center:	destinationRectangle = D2D1::RectF(x1 - x2 / 2.0f, y1 - y2 / 2.0f, x1 + x2 / 2.0f, y1 + y2 / 2.0f); break;
				case Radius:
				{
					const float width = x2 * 2.0f, height = y2 * 2.0f;
					destinationRectangle = D2D1::RectF(x1 - width / 2.0f, y1 - height / 2.0f, x1 + width / 2.0f, y1 + height / 2.0f);
				} break;
			}
			
			target.DrawBitmap(
				bitmap,
				destinationRectangle,
				(float)style.TextureOpacity / 255.0f,
				(D2D1_BITMAP_INTERPOLATION_MODE)style.TextureSampleMode,
				D2D1::RectF(sourceRectangle.Left, sourceRectangle.Top, sourceRectangle.Left + sourceRectangle.Width, sourceRectangle.Top + sourceRectangle.Height)
			);
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Geometry(const Shape& shape)
	{
		if (ID2D1Geometry* geometry = shape.GetGeometry())
		{
			ID2D1RenderTarget& target = GetRenderTarget();
			const RenderStyle& style = GetRenderStyle();
			target.SetTransform(reinterpret_cast<const D2D1_MATRIX_3X2_F&>(GetTransform().GetTransform()));

			if (ID2D1Brush* brush = style.ActiveFill)
			{
				target.FillGeometry(geometry, brush);
			}

			if (ID2D1Brush* brush = style.ActiveStroke)
			{
				target.DrawGeometry(geometry, brush, style.StrokeWeight, style.StrokeStyle.GetStyleStroke());
			}
		}
	}

	////////////////////////////////////////////////////////////
	const RenderStyle& RenderTarget::GetRenderStyle() const
	{
		return styles.top();
	}

	////////////////////////////////////////////////////////////
	RenderStyle& RenderTarget::GetRenderStyle()
	{
		return styles.top();
	}

	////////////////////////////////////////////////////////////
	Shape& RenderTarget::GetGeometry()
	{
		return geometry;
	}

	////////////////////////////////////////////////////////////
	const Shape& RenderTarget::GetGeometry() const
	{
		return geometry;
	}
}
