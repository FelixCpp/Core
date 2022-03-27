// 
// Sleep.hpp
// Core
// 
// Created by Felix Busch on 07.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Time.hpp>

namespace Core
{

	////////////////////////////////////////////////////////////
	/// \brief Define static class that provides sleeping
	///		   functionality
	/// 
	////////////////////////////////////////////////////////////
	class Sleep
	{
	public:
		////////////////////////////////////////////////////////////
		/// \brief Stops the program for specified duration
		///
		/// \param duration The duration to sleep
		///
		////////////////////////////////////////////////////////////
		static void Perform(const Time& duration);
	};

}