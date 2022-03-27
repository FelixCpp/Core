// 
// Application.cpp
// Core
// 
// Created by Felix Busch on 15.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#include <Core/Application/Application.hpp>
#include <Core/Application/Globals.hpp>

#include <Core/System/FinalAction.hpp>
#include <Core/System/Error.hpp>
#include <Core/System/Sleep.hpp>

#include <thread>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Client-side defined method that returns its
	///		   implementation of a sketch
	/// 
	////////////////////////////////////////////////////////////
	extern Sketch* CreateSketch();

	////////////////////////////////////////////////////////////
	/// Link static members
	/// 
	////////////////////////////////////////////////////////////
	Application* Application::Instance = nullptr;

	////////////////////////////////////////////////////////////
	Application::Application():
		IsRendering(false),
		AutoCloseEnabled(true),
		TargetFps(60),
		FramesPerSecond(0),
		FrameCount(0),
		FpsTime(Time::Zero),
		FrameTimer(Stopwatch::StartNew())
	{
	}

	////////////////////////////////////////////////////////////
	void Application::Start()
	{
		// open the window
		if(!Window.Open(1280, 720, L"Core Application Window"))
		{
			// get out immediately
			return;
		}

		const FinalAction windowDeletion = [&] { Window.Destroy(); };
		GlobalUpdatingService globals;

		Window.AddEventListener(globals);
		Window.AddEventListener(*this);

		IsRendering = true;
		std::thread renderThread(&Application::RenderThreadImpl, this);

		while(IsRendering)
		{
			// queue up & dispatch the events
			Window.DispatchEvents();
		}

		if(renderThread.joinable())
		{
			renderThread.join();
		}
	}

	////////////////////////////////////////////////////////////
	void Application::Exit()
	{
		IsRendering = false;
	}

	////////////////////////////////////////////////////////////
	void Application::OnEvent(const WindowEvent& event)
	{
		if (!AutoCloseEnabled)
			return;

		switch (event.Type)
		{
			case WindowEvent::Closed: Exit(); break;
			case WindowEvent::KeyReleased: if (event.Key.Code == KeyCode::Escape) Exit(); break;
			default: break;
		}
	}

	////////////////////////////////////////////////////////////
	void Application::RenderThreadImpl()
	{
		// initialize the graphics
		if(!Graphics.Create(Window))
		{
			IsRendering = false;
			return;
		}
		const FinalAction graphicsDeletion = [&] { Graphics.Destroy(); };

		// create sketch
		if(Sketch.reset(CreateSketch()); !Sketch)
		{
			Err() << "CreateSketch() returned nullptr" << std::endl;
			IsRendering = false;
			return;
		}

		const FinalAction sketchDeletion = [&] { Sketch.reset(); };


		// add event listeners
		Window.AddEventListener(Graphics);
		Window.AddEventListener(*Sketch);
		
		// load assets
		if(!Sketch->OnPreload())
		{
			Err() << "OnPreload() returned false";
			IsRendering = false;
			return;
		}

		Sketch->OnSetup();

		// start game timer
		Stopwatch gameTimer = Stopwatch::StartNew();

		while (IsRendering)
		{
			const Time deltaTime = gameTimer.Restart();

			if (Graphics.BeginDraw())
			{
				// render user data
				Sketch->OnDraw(deltaTime.ToSeconds<float>());

				if(!Graphics.EndDraw())
				{
					IsRendering = false;
					break;
				}
			}
			
			HandleFps(deltaTime);
		}

		Sketch->OnDestroy();

		// remove all event listeners
		Window.RemoveAllEventListeners();
	}

	void Application::HandleFps(const Time& deltaTime)
	{
		++FrameCount;
		FpsTime += deltaTime;

		// calculate the frames per second
		if(FpsTime >= Seconds(1.f))
		{
			FramesPerSecond = (u32)((float)FrameCount / FpsTime.ToSeconds<float>());
			FrameCount = 0;
			FpsTime = Time::Zero;
		}

		if(TargetFps != 0)
		{
			const Time limitTime = Seconds(1.0f / (float)TargetFps);
			const Time sleepDuration = limitTime - FrameTimer.GetElapsedTime();
			
			// sleep for the given duration
			Sleep::Perform(sleepDuration);

			FrameTimer.Restart();
		}
	}
}
