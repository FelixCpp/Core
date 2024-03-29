﻿// 
// MouseCursor.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Window/Types.hpp>
#include <Core/System/Value2.hpp>

#include <memory>
#include <filesystem>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define mouse cursor.
	///
	///	The mouse cursor is intended to be used as a cursor
	///	for a window. It can be loaded from a file or raw
	///	memory.
	/// 
	////////////////////////////////////////////////////////////
	class MouseCursor
	{
	public:

		enum System
		{
			////////////////////////////////////////////////////////////
			/// Standard arrow and small hourglass
			/// 
			////////////////////////////////////////////////////////////
			AppStarting,

			////////////////////////////////////////////////////////////
			/// Standard arrow
			/// 
			////////////////////////////////////////////////////////////
			Arrow,

			////////////////////////////////////////////////////////////
			/// Crosshair
			/// 
			////////////////////////////////////////////////////////////
			Cross,

			////////////////////////////////////////////////////////////
			/// Hand
			/// 
			////////////////////////////////////////////////////////////
			Hand,

			////////////////////////////////////////////////////////////
			/// Arrow and question mark
			/// 
			////////////////////////////////////////////////////////////
			Help,

			////////////////////////////////////////////////////////////
			/// I-beam
			/// 
			////////////////////////////////////////////////////////////
			IBeam,

			////////////////////////////////////////////////////////////
			/// Obsolete for applications marked version 4.0 or later.
			/// 
			////////////////////////////////////////////////////////////
			Icon,

			////////////////////////////////////////////////////////////
			/// Slashed circle
			/// 
			////////////////////////////////////////////////////////////
			No,

			////////////////////////////////////////////////////////////
			/// Obsolete for applications marked version 4.0 or later.
			///	Use SizeAll
			/// 
			////////////////////////////////////////////////////////////
			Size,

			////////////////////////////////////////////////////////////
			/// Four-pointed arrow pointing north, south, ease and west.
			/// 
			////////////////////////////////////////////////////////////
			SizeAll,

			////////////////////////////////////////////////////////////
			/// Double-pointed arrow pointing northeast and southwest
			/// 
			////////////////////////////////////////////////////////////
			SizeNESW,

			////////////////////////////////////////////////////////////
			/// Double-pointed arrow pointing north and south
			/// 
			////////////////////////////////////////////////////////////
			SizeNS,

			////////////////////////////////////////////////////////////
			/// Double-pointed arrow pointing northwest and southeast
			/// 
			////////////////////////////////////////////////////////////
			SizeNWSE,

			////////////////////////////////////////////////////////////
			/// Double-pointed arrow pointing west and east
			/// 
			////////////////////////////////////////////////////////////
			SizeWE,

			////////////////////////////////////////////////////////////
			/// Vertical arrow
			/// 
			////////////////////////////////////////////////////////////
			UpArrow,

			////////////////////////////////////////////////////////////
			/// Hourglass
			/// 
			////////////////////////////////////////////////////////////
			Wait
		};

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		///	Initializes to an arrow cursor.
		/// 
		////////////////////////////////////////////////////////////
		MouseCursor();
		
		////////////////////////////////////////////////////////////
		/// \brief Loads the cursor from a file using the STB
		///		   imaging library.
		///
		///	\param filepath The file location on disk.
		/// \param hotspot The center offset to render from
		///
		///	\return True if the cursor has been loaded successfully,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromFile(const std::filesystem::path& filepath, const UInt2& hotspot);

		////////////////////////////////////////////////////////////
		/// \brief Loads the cursor from a pre-loaded system asset.
		///
		///	\param system The enumeration that describes the cursor.
		///
		///	\return True if the cursor has been loaded successfully,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromSystem(System system);

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying cursor handle
		/// 
		////////////////////////////////////////////////////////////
		CursorHandle GetCursorHandle() const;

	private:

		////////////////////////////////////////////////////////////
		/// \brief We're implementing the PImpl-pattern here
		///		   so we don't need to include the dirty
		///		   windows header files.
		/// 
		////////////////////////////////////////////////////////////
		class Impl;

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::shared_ptr<Impl> impl;	///< Pointer to implementation
		
	};
}