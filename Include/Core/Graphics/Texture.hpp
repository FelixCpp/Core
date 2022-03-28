﻿// 
// Texture.hpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/String.hpp>
#include <Core/System/Types.hpp>
#include <Core/System/Value2.hpp>

#include <memory>

////////////////////////////////////////////////////////////
/// Forward declaration
/// 
////////////////////////////////////////////////////////////
struct ID2D1Bitmap;

namespace Core
{

	////////////////////////////////////////////////////////////
	/// \brief Define texture class.
	///
	///	A texture does not hold the pixel information of itself
	///	but can be rendered to the screen anyways.
	/// 
	////////////////////////////////////////////////////////////
	class Texture
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////
		Texture();

		////////////////////////////////////////////////////////////
		/// \brief Load the texture from a file.
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromFile(const String& filepath);

		////////////////////////////////////////////////////////////
		/// \brief Copy a part of the texture into a new resource.
		///
		///	\param x		The x-coordinate to start copying from
		///	\param y		The y-coordinate to start copying from
		///	\param width	The width of the rectangle to copy
		///	\param height	The height of the rectangle to copy
		/// 
		////////////////////////////////////////////////////////////
		Texture Get(i32 x, i32 y, i32 width, i32 height) const;
		
		////////////////////////////////////////////////////////////
		/// \brief Get the underlying bitmap handle from Direct2D
		/// 
		////////////////////////////////////////////////////////////
		ID2D1Bitmap* GetBitmap() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the size of the texture
		/// 
		////////////////////////////////////////////////////////////
		const Float2& GetSize() const;

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
		std::shared_ptr<Impl>	impl;	///< Pointer to implementation
		Float2					size;	///< The size of the bitmap

	};
	
}