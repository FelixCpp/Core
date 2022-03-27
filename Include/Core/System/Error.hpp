// 
// Error.hpp
// Core
// 
// Created by Felix Busch on 15.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#pragma once

#include <ostream>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Internal error logging buffer to write messages
	///		   into
	/// 
	////////////////////////////////////////////////////////////
	std::ostream& Err();
}