// 
// RenderTarget.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/RenderTarget.hpp>
#include <Core/Graphics/Shape.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d2d1.h>

namespace Core
{

	////////////////////////////////////////////////////////////
	RenderTarget::RenderTarget()
	{
		// push the initial rendering style
		styles.push(RenderStyle());

		// paint white by default.
		//Fill(Color::White);
		//Stroke(Color::White);
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
		const RenderStyle& style = GetRenderStyle();
		ID2D1RenderTarget& target = GetRenderTarget();

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
		const RenderStyle& style = GetRenderStyle();

		if (ID2D1Brush* brush = style.ActiveStroke)
		{
			GetRenderTarget().DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), brush, style.StrokeWeight, style.StrokeStyle.GetStyleStroke());
		}
	}

	////////////////////////////////////////////////////////////
	void RenderTarget::Geometry(const Shape& shape)
	{
		if (ID2D1Geometry* geometry = shape.GetGeometry())
		{
			ID2D1RenderTarget& target = GetRenderTarget();
			const RenderStyle& style = GetRenderStyle();

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
}
