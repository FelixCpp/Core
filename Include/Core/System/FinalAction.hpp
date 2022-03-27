// 
// FinalAction.hpp
// Core
// 
// Created by Felix Busch on 20.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define action callback on destruction wrapper
	/// 
	////////////////////////////////////////////////////////////
	template<typename T>
	class FinalAction
	{
	public:
		////////////////////////////////////////////////////////////
		/// \brief Constructs a FinalAction object with associated function
		///		   to call on destruction
		/// 
		////////////////////////////////////////////////////////////
		FinalAction(const T& lambda):
			function(lambda),
			invokable(true)
		{
		}

		FinalAction(const FinalAction& other) = delete;
		FinalAction & operator = (const FinalAction& other) = delete;

		////////////////////////////////////////////////////////////
		/// \brief Move constructor that takes over the ownership
		/// 
		////////////////////////////////////////////////////////////
		FinalAction(FinalAction&& other) noexcept :
			function(other.function),
			invokable(other.invokable)
		{
			other.invokable = false;
		}

		////////////////////////////////////////////////////////////
		/// \brief Move assignment that takes over the ownership
		/// 
		////////////////////////////////////////////////////////////
		FinalAction& operator = (FinalAction&& other) noexcept
		{
			function = other.function;
			invokable = other.invokable;
			other.invokable = false;
			return *this;
		}

		////////////////////////////////////////////////////////////
		/// \brief Destructor that calls the lambda
		/// 
		////////////////////////////////////////////////////////////
		~FinalAction()
		{
			if(invokable)
				function();
		}

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		T function;		//!< The lambda that is called in the destructor
		bool invokable;	//!< A boolean to keep track of whether we're the owner if the function

	};

}