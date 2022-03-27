// 
// EntryPoint.cpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Application/Application.hpp>
#include <Core/Window/Dpi.hpp>
#include <Core/Graphics/Factories.hpp>

#include <new>

////////////////////////////////////////////////////////////
/// \brief Entry point
/// 
////////////////////////////////////////////////////////////
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	// Initialize the factories and COM-interface
	if(!Core::Factories::Setup())
	{
		return EXIT_FAILURE;
	}

	// we don't really care about the configuration of the dpi scale
	[[maybe_unused]] const bool success = Core::Dpi::ConfigureDpi();
	
	// create application
	if(auto* app = new(std::nothrow) Core::Application())
	{
		// link app instance
		Core::Application::Instance = app;

		// start the application
		app->Start();

		// cleanup
		delete app;
	}

	// release the factory handles
	Core::Factories::Cleanup();
	
	return EXIT_SUCCESS;
}