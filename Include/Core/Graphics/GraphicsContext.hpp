// 
// GraphicsContext.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/IEventListener.hpp>
#include <Core/System/Types.hpp>

#include <Core/Window/WindowEvent.hpp>

#include <Core/Graphics/RenderTarget.hpp>

#include <memory>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// Forward declarations
	/// 
	////////////////////////////////////////////////////////////
	class Window;
	
	////////////////////////////////////////////////////////////
	/// \brief Define graphics context wrapper for Direct2D.
	///
	///	This class wraps Direct2D and implements
	///	the necessary steps in order to interact with
	///	the Core API.
	///
	///	In order to keep track of window size changes we
	///	implement the event-listener interface to get notified
	///	when the window size has changed.
	/// 
	////////////////////////////////////////////////////////////
	class GraphicsContext final : public RenderTarget, public IEventListener<WindowEvent>
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////
		GraphicsContext();

		////////////////////////////////////////////////////////////
		/// \brief Default destructor
		/// 
		////////////////////////////////////////////////////////////
		virtual ~GraphicsContext() override = default;

		////////////////////////////////////////////////////////////
		/// \brief Creates the graphics context associated to the
		///		   window.
		///
		/// \param window The window to render on.
		///
		///	\return True if the context has been setup successfully,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool Create(const Window& window);

		////////////////////////////////////////////////////////////
		/// \brief Destroys the graphics context.
		/// 
		////////////////////////////////////////////////////////////
		void Destroy();

		////////////////////////////////////////////////////////////
		/// \brief Prepare rendering frame.
		///
		///	Ensures that the projection surface is the same
		///	size as the window area. This has to be done each time
		///	the window has been resized.
		///
		///	\return True if the render target has been resized
		///			as expected, false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool BeginDraw();

		////////////////////////////////////////////////////////////
		/// \brief Stops listening on rendering commands.
		///
		///	\return True if the content has been presented
		///			on screen, false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool EndDraw();

		////////////////////////////////////////////////////////////
		/// \brief Callback method for window events.
		///
		///	We have to make sure we resize the underlying
		///	render target on the same thread as the
		///	rendering is done.
		///
		///	\param event Additional information about the event.
		///
		////////////////////////////////////////////////////////////
		virtual void OnEvent(const WindowEvent& event) override;
		
		////////////////////////////////////////////////////////////
		/// \brief Overwritten getter-method for the underlying
		///		   render target.
		/// 
		////////////////////////////////////////////////////////////
		virtual ID2D1RenderTarget& GetRenderTarget() override;

	private:

		////////////////////////////////////////////////////////////
		/// \brief Define size-cache structure.
		///
		///	This is used to keep track of the window size
		///	after the window has been resized.
		/// 
		////////////////////////////////////////////////////////////
		struct SizeCache
		{
			u32		ProjectionWidth;
			u32		ProjectionHeight;
			bool	NeedsResize;
		};

		////////////////////////////////////////////////////////////
		/// \brief Use PImpl-pattern so we don't need to include
		///		   the dirty Direct2D headers.
		/// 
		////////////////////////////////////////////////////////////
		class Impl;
		
		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::shared_ptr<Impl>							impl;			///< Pointer to implementation class.
		SizeCache										sizeCache;		///< A cache for sizing information for the next frame.
	};
}
