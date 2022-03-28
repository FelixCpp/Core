// 
// Shape.cpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/Shape.hpp>
#include <Core/System/Error.hpp>
#include <Core/Application/Factories.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d2d1.h>
#include <wrl/client.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define concrete implementation for the Shape
	/// 
	////////////////////////////////////////////////////////////
	class Shape::Impl
	{
	public:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		Microsoft::WRL::ComPtr<ID2D1GeometrySink>	Sink		= nullptr;
		Microsoft::WRL::ComPtr<ID2D1PathGeometry>	Geometry	= nullptr;
		bool										First		= false;

	};

	////////////////////////////////////////////////////////////
	Shape::Shape():
		impl(std::make_shared<Impl>()),
		isBuilding(false)
	{
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::Begin()
	{
		if(isBuilding)
		{
			Err() << "The shape is already in building mode." << std::endl;
			isBuilding = false;
			return *this;
		}

		HRESULT success = Factories::D2DFactory->CreatePathGeometry(&impl->Geometry);
		if(FAILED(success))
		{
			Err() << "Failed to create an ID2D1PathGeometry instance." << std::endl;
			isBuilding = false;
			return *this;
		}

		success = impl->Geometry->Open(&impl->Sink);
		if(FAILED(success))
		{
			Err() << "Failed to open the geometry sink instance." << std::endl;
			isBuilding = false;
			return *this;
		}

		impl->First = true;
		isBuilding = true;
		return *this;
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::AddVertex(float x, float y)
	{
		if(!isBuilding)
		{
			Err() << "Failed to add a vertex to the shape. Make sure to call AddVertex() only between Begin() and End()" << std::endl;
			return *this;
		}

		if(impl->First)
		{
			impl->Sink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
			impl->First = false;
		} else
		{
			impl->Sink->AddLine(D2D1::Point2F(x, y));
		}

		return *this;
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::AddVertex(const Float2& point)
	{
		return AddVertex(point.X, point.Y);
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::AddBezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		if (!isBuilding)
		{
			Err() << "Failed to add a vertex to the shape. Make sure to call AddBezier() only between Begin() and End()" << std::endl;
			return *this;
		}

		if (impl->First)
		{
			impl->Sink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_FILLED);
			impl->First = false;
		}

		impl->Sink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), D2D1::Point2F(x3, y3)));
		return *this;
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::AddBezier(const Float2& start, const Float2& center, const Float2& end)
	{
		return AddBezier(start.X, start.Y, center.X, center.Y, end.X, end.Y);
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::AddQuadraticBezier(float x1, float y1, float x2, float y2)
	{
		if (!isBuilding)
		{
			Err() << "Failed to add a vertex to the shape. Make sure to call AddQuadraticBezier() only between Begin() and End()" << std::endl;
			return *this;
		}

		if (impl->First)
		{
			impl->Sink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_FILLED);
			impl->First = false;
		}

		impl->Sink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2)));
		return *this;
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::AddQuadraticBezier(const Float2& start, const Float2& end)
	{
		return AddQuadraticBezier(start.X, start.Y, end.X, end.Y);
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::End(ShapeEnd style)
	{
		if (impl->First)
			return *this;

		if(!isBuilding)
		{
			Err() << "Unacceptable method call to End(). Make sure to call Begin() before calling End()" << std::endl;
			return *this;
		}

		impl->Sink->EndFigure((D2D1_FIGURE_END)style);

		const HRESULT success = impl->Sink->Close();
		if(FAILED(success))
		{
			Err() << "Failed to close the geometry sink" << std::endl;
			return *this;
		}

		isBuilding = false;

		return *this;
	}

	////////////////////////////////////////////////////////////
	bool Shape::IsBuilding() const
	{
		return isBuilding;
	}

	////////////////////////////////////////////////////////////
	ID2D1Geometry* Shape::GetGeometry() const
	{
		return impl->Geometry.Get();
	}

}