// 
// Shape.hpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Value2.hpp>
#include <Core/Graphics/ShapeProperties.hpp>

#include <memory>

////////////////////////////////////////////////////////////
/// Forward declaration
/// 
////////////////////////////////////////////////////////////
struct ID2D1Geometry;

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define shape class to build complex geometries.
	/// 
	////////////////////////////////////////////////////////////
	class Shape
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		Shape();

		////////////////////////////////////////////////////////////
		/// \brief Starts listening on building commands
		///
		///	\return The updated version of itself.
		/// 
		////////////////////////////////////////////////////////////
		Shape& Begin();

		////////////////////////////////////////////////////////////
		/// \brief Adds a vertex to the shape.
		///
		///	The vertices will be connected via lines.
		///	This method can only be called surrounded with Begin()
		///	and End().
		///
		///	\param x The x-coordinate of the vertex
		///	\param y The y-coordinate of the vertex
		///
		///	\return The updated version of itself.
		/// 
		////////////////////////////////////////////////////////////
		Shape& AddVertex(float x, float y);
		Shape& AddVertex(const Float2& point);

		////////////////////////////////////////////////////////////
		/// \brief Adds a bezier segment to the shape.
		///
		///	The vertices will be connected via lines.
		///	This method can only be called surrounded with Begin()
		///	and End().
		///
		///	\param x1 x-coordinate of the first point
		///	\param y1 y-coordinate of the first point
		///	\param x2 x-coordinate of the second point
		///	\param y2 y-coordinate of the second point
		///	\param x3 x-coordinate of the third point
		///	\param y3 y-coordinate of the third point
		/// 
		////////////////////////////////////////////////////////////
		Shape& AddBezier(float x1, float y1, float x2, float y2, float x3, float y3);
		Shape& AddBezier(const Float2& start, const Float2& center, const Float2& end);

		////////////////////////////////////////////////////////////
		/// \brief Adds a quadratic bezier segment to the shape.
		///
		///	The vertices will be connected via lines.
		///	This method can only be called surrounded with Begin()
		///	and End().
		///
		///	\param x1 x-coordinate of the first point
		///	\param y1 y-coordinate of the first point
		///	\param x2 x-coordinate of the second point
		///	\param y2 y-coordinate of the second point
		/// 
		////////////////////////////////////////////////////////////
		Shape& AddQuadraticBezier(float x1, float y1, float x2, float y2);
		Shape& AddQuadraticBezier(const Float2& start, const Float2& end);

		////////////////////////////////////////////////////////////
		/// \brief Stops listening on building commands.
		///
		/// \param style The style how to end the shape geometry.
		///
		///	\return The updated version of itself.
		///
		////////////////////////////////////////////////////////////
		Shape& End(ShapeEnd style);

		////////////////////////////////////////////////////////////
		/// \brief Tell whether the shape is currently being built
		///		   up or not.
		///
		///	A shape is build between Begin() and End().
		/// 
		////////////////////////////////////////////////////////////
		bool IsBuilding() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the wrapped geometry object from the
		///		   implementation.
		/// 
		////////////////////////////////////////////////////////////
		ID2D1Geometry* GetGeometry() const;

	private:

		////////////////////////////////////////////////////////////
		/// \brief PImpl pattern - you know the drill
		/// 
		////////////////////////////////////////////////////////////
		class Impl;

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::shared_ptr<Impl>	impl;		///< Pointer to implementation
		bool					isBuilding;	///< Whether the shape is currently being built up or not.
	};
}