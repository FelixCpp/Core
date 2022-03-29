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
		
	};

	////////////////////////////////////////////////////////////
	Shape::Shape():
		impl(std::make_shared<Impl>()),
		fillMode(FillMode::Alternate),
		segmentFlags(PathSegment::None),
		isBuilding(false)
	{
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::SetSegmentFlags(PathSegment flags)
	{
		segmentFlags = flags;

		if(impl->Sink != nullptr)
		{
			impl->Sink->SetSegmentFlags((D2D1_PATH_SEGMENT)flags);
		}

		return *this;
	}

	////////////////////////////////////////////////////////////
	Shape::PathSegment Shape::GetSegmentFlags() const
	{
		return segmentFlags;
	}

	////////////////////////////////////////////////////////////
	Shape& Shape::SetFillMode(FillMode mode)
	{
		fillMode = mode;

		if(impl->Sink != nullptr)
		{
			impl->Sink->SetFillMode((D2D1_FILL_MODE)mode);
		}

		return *this;
	}

	////////////////////////////////////////////////////////////
	Shape::FillMode Shape::GetFillMode() const
	{
		return fillMode;
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

		const HRESULT success = Factories::D2DFactory->CreatePathGeometry(&impl->Geometry);
		if(FAILED(success))
		{
			Err() << "Failed to create an ID2D1PathGeometry instance." << std::endl;
			isBuilding = false;
			return *this;
		}

		impl->Sink.Reset();
		
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

		if(const auto& sink = impl->Sink; !sink)
		{
			if(!OpenGeometrySink())
			{
				isBuilding = false;
				return *this;
			}
			
			sink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
		} else
		{
			sink->AddLine(D2D1::Point2F(x, y));
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

		if (const auto& sink = impl->Sink; !sink)
		{
			if (!OpenGeometrySink())
			{
				isBuilding = false;
				return *this;
			}
			
			sink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_FILLED);
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

		if (const auto& sink = impl->Sink; !sink)
		{
			if (!OpenGeometrySink())
			{
				isBuilding = false;
				return *this;
			}
			
			sink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_FILLED);
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
		if(!isBuilding)
		{
			Err() << "Unacceptable method call to End(). Make sure to call Begin() before calling End()" << std::endl;
			return *this;
		}

		// make sure to call this only if the sink has been opened
		if (ID2D1GeometrySink* sink = impl->Sink.Get())
		{
			sink->EndFigure((D2D1_FIGURE_END)style);

			const HRESULT success = sink->Close();
			if (FAILED(success))
			{
				Err() << "Failed to close the geometry sink" << std::endl;
				return *this;
			}
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

	bool Shape::IsRenderable() const
	{
		return impl->Geometry != nullptr && impl->Sink != nullptr;
	}

	////////////////////////////////////////////////////////////
	/// \brief Tries to open the geometry sink object.
	///
	///	\return True if the object has been opened successfully,
	///			false otherwise.
	/// 
	////////////////////////////////////////////////////////////
	bool Shape::OpenGeometrySink()
	{
		if (ID2D1PathGeometry* geometry = impl->Geometry.Get())
		{
			if (auto& sink = impl->Sink)
			{
				Err() << "The geometry sink is already open." << std::endl;
				return true;
			} else
			{
				if (FAILED(geometry->Open(&sink)))
				{
					Err() << "Failed to open the geometry sink instance." << std::endl;
					return false;
				}

				sink->SetSegmentFlags((D2D1_PATH_SEGMENT)segmentFlags);
				sink->SetFillMode((D2D1_FILL_MODE)fillMode);
				return true;
			}
		}

		return false;
	}
}
