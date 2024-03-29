﻿// 
// WindowIcon.hpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Window/Types.hpp>

#include <memory>
#include <filesystem>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define window icon class.
	///
	///	An instance of this class can be used to set
	///	the icon for a window.
	/// 
	////////////////////////////////////////////////////////////
	class WindowIcon
	{
	public:

		enum System
		{
			////////////////////////////////////////////////////////////
			/// Default application icon.
			/// 
			////////////////////////////////////////////////////////////
			Application,

			////////////////////////////////////////////////////////////
			/// Asterisk icon. Same as Information
			/// 
			////////////////////////////////////////////////////////////
			Asterisk,

			////////////////////////////////////////////////////////////
			/// Hand-shaped icon.
			/// 
			////////////////////////////////////////////////////////////
			Error,

			////////////////////////////////////////////////////////////
			/// Exclamation point icon. Same as Warning
			/// 
			////////////////////////////////////////////////////////////
			Exclamation,

			////////////////////////////////////////////////////////////
			/// Hand-shaped icon. Same as Error
			/// 
			////////////////////////////////////////////////////////////
			Hand,

			////////////////////////////////////////////////////////////
			/// Asterisk icon.
			/// 
			////////////////////////////////////////////////////////////
			Information,

			////////////////////////////////////////////////////////////
			/// Question mark icon.
			/// 
			////////////////////////////////////////////////////////////
			Question,

			////////////////////////////////////////////////////////////
			/// Security shield icon.
			/// 
			////////////////////////////////////////////////////////////
			Shield,

			////////////////////////////////////////////////////////////
			/// Exclamation point icon.
			/// 
			////////////////////////////////////////////////////////////
			Warning,

			////////////////////////////////////////////////////////////
			/// Default application icon.
			///	Windows 2000: Windows logo icon.
			/// 
			////////////////////////////////////////////////////////////
			WinLogo
		};

		////////////////////////////////////////////////////////////
		/// \brief Construct a default window icon.
		/// 
		////////////////////////////////////////////////////////////
		WindowIcon();
		
		////////////////////////////////////////////////////////////
		/// \brief Load the icon from a file.
		///
		///	\param filepath The file location on disk.
		///
		///	\return True if the icon has been loaded successfully,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromFile(const std::filesystem::path& filepath);

		////////////////////////////////////////////////////////////
		/// \brief Loads the icon from pre-defined system assets.
		///
		///	\param system The icon identification.
		///
		///	\return True if the icon has been loaded successfully,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromSystem(System system);

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying icon handle.
		/// 
		////////////////////////////////////////////////////////////
		IconHandle GetIconHandle() const;

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
		std::shared_ptr<Impl> impl;	///< Pointer to implementation

	};
	
}