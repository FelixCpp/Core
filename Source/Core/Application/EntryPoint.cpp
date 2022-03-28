// 
// EntryPoint.cpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Application/Application.hpp>
#include <Core/Window/Dpi.hpp>
#include <Core/Application/Factories.hpp>

#include <new>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

////////////////////////////////////////////////////////////
/// \brief Disables the exit-button of the console window.
/// 
////////////////////////////////////////////////////////////
void DisableConsoleCloseButton()
{
	const HWND consoleWindow = GetConsoleWindow();
	const HMENU consoleMenu = GetSystemMenu(consoleWindow, FALSE);
	EnableMenuItem(consoleMenu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}

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

	// avoid exiting the application through the application.
	// since that leads to problems during the exit process.
	DisableConsoleCloseButton();

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