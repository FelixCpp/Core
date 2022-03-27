//
// Application.hpp
// Core
// 
// Created by Felix Busch on 15.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#pragma once

#include <Core/Application/Sketch.hpp>
#include <Core/Window/Window.hpp>
#include <Core/Graphics/GraphicsContext.hpp>

#include <Core/System/Types.hpp>
#include <Core/System/Time.hpp>
#include <Core/System/Stopwatch.hpp>

#include <memory>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Application that manages life cycles for the
	///		   sketch
	/// 
	////////////////////////////////////////////////////////////
	class Application : public IEventListener<WindowEvent>
	{
	public:

		////////////////////////////////////////////////////////////
		/// Public static member data
		/// 
		////////////////////////////////////////////////////////////
		static Application* Instance;

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		Application();
		
		////////////////////////////////////////////////////////////
		/// \brief Create the dependencies and launch the sketch
		///
		/// Dependencies is everything around the sketch:
		///		- Window
		///		- Audio
		///		- Graphics
		///		- Networking
		///		etc.
		///
		////////////////////////////////////////////////////////////
		void Start();

		////////////////////////////////////////////////////////////
		/// \brief Start the exit process
		/// 
		////////////////////////////////////////////////////////////
		void Exit();

		////////////////////////////////////////////////////////////
		/// \brief Event callback for window messages
		/// 
		////////////////////////////////////////////////////////////
		virtual void OnEvent(const WindowEvent& event) override;

	private:

		////////////////////////////////////////////////////////////
		/// \brief Method that runs on the rendering thread
		/// 
		////////////////////////////////////////////////////////////
		void RenderThreadImpl();

		////////////////////////////////////////////////////////////
		/// \brief Counts and calculates the frames per second
		///
		/// \param deltaTime The time elapsed since last frame
		///
		////////////////////////////////////////////////////////////
		void HandleFps(const Time& deltaTime);

	public:
		
		////////////////////////////////////////////////////////////
		/// Public member data
		///
		////////////////////////////////////////////////////////////
		std::unique_ptr<Sketch>	Sketch;				///< The client sketch
		Window					Window;				///< The window to render on
		GraphicsContext			Graphics;			///< The graphics context that is used for rendering.
		std::atomic_bool		IsRendering;		///< Keep the game loop alive
		bool					AutoCloseEnabled;	///< State whether to close the app automatically on window close event
		u32						TargetFps;			///< The number of frames per second the application wants to reach
		u32						FramesPerSecond;	///< The actual number of frames per second
		u32						FrameCount;			///< The number of frames counted since last time calculating the fps
		Time					FpsTime;			///< Timer that is used to calculate the frames per second
		Stopwatch				FrameTimer;			///< The time elapsed since last time the application slept to keep the frame rate limited

	};
}