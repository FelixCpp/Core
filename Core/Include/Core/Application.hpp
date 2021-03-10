#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>
#include <Core/Rendering/RenderWindow.hpp>
#include <Core/Rendering/Renderers/RendererType.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <type_traits>
#include <string>

namespace Core
{

	class Renderer;
	class RenderStateManager;

	/// <summary>
	/// This class is used to handle everything around Setup and Draw
	/// </summary>
	class LoopManager {
	public:

		/// <summary>
		/// Default virtual destructor
		/// </summary>
		virtual ~LoopManager() = default;

		/// <summary>
		/// A virtual method. The declaration
		/// does not force the user to implement
		/// this method since it's not that important.
		/// 
		/// This method gets called once at the beginning of
		/// the sketch.
		/// </summary>
		virtual void Setup() {}

		/// <summary>
		/// A virtual method. The declaration
		/// does not force the user to implement
		/// this method since it's not that important
		/// 
		/// This method gets called over an over again (every frame)
		/// based on the drawing-state.
		/// 
		/// It can be toggled on and off using
		/// StartDrawing and PauseDrawing
		/// </summary>
		virtual void Draw() {}

		/// <summary>
		/// Starts calling the Draw method.
		/// </summary>
		void StartDrawing();

		/// <summary>
		/// Stops calling the Draw method.
		/// </summary>
		void PauseDrawing();

		/// <summary>
		/// Calls the Draw method even
		/// if the drawing is paused.
		/// </summary>
		void Redraw();

		/// <summary>
		/// Returns true if the drawing
		/// is active
		/// </summary>
		bool CanDraw() const;

		/// <summary>
		/// This method gets called
		/// after every Draw() call
		/// </summary>
		virtual void OnFrameProcessed() {}

	protected:

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="renderer">passes the given instance into its member varaible</param>
		explicit LoopManager(LateRef<Renderer> renderer);

	private:

		/// <summary>
		/// Controls calling the Draw method.
		/// </summary>
		bool drawingActive;

		/// <summary>
		/// A references-pointer to a Renderer given
		/// by the Application class
		/// </summary>
		LateRef<Renderer> renderer;

	};

	class Application : public RenderWindow, public LoopManager {
	public:

		/// <summary>
		/// Default virtual destructor
		/// </summary>
		virtual ~Application();

		/* exits the application (calls close() on the Window base) */
		void Exit();

		/* call this function to start the sketch */
		template<class TDerived, typename ... TArgs, typename = std::enable_if_t<std::is_base_of_v<Application, TDerived>>>
		static void Launch(const TArgs & ... arguments)
		{
			// We're initializing the variables which needs to be alive outside
			// the lifetime of the instance variable
			GlobalInit();
			
			// instance needs to be destroyed before calling
			// GlobalDestroy() !
			{
				TDerived instance(std::forward<decltype(arguments)>(arguments)...);
				instance.StartSketch();
			}

			// We destroy the variables
			GlobalDestroy();
		}

	protected:

		/// <summary>
		/// Creates a brand new Application
		/// </summary>
		/// <param name="width">width of the window</param>
		/// <param name="height">height of the window</param>
		/// <param name="title">title of the window</param>
		/// <param name="type">type of the Renderer</param>
		explicit Application(Int32 width = 200, Int32 height = 200, const std::string & title = "Core - Application", RendererType type = RendererType::WindowRenderer);

	private:

		/// <summary>
		/// Starts the sketch
		/// </summary>
		void StartSketch();

		/// <summary>
		/// Initializes the variables
		/// who needs to be alive
		/// outside of the the Application's
		/// lifetime
		/// </summary>
		static void GlobalInit();

		/// <summary>
		/// Destroys the global variables
		/// </summary>
		static void GlobalDestroy();

	private:

		/// <summary>
		/// Since this class contains raw pointers
		/// we do not allow copying or moving
		/// this class to another instance
		/// </summary>
		
		Application(const Application &) = delete;
		Application(Application &&) = delete;
		Application & operator=(const Application &) = delete;
		Application & operator=(Application &&) = delete;

	private:

		/* instance of a Renderer */
		Renderer * renderer;

		/* instance of a RenderStateManager */
		RenderStateManager * rsm;

	};

}