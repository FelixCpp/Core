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

	class Application : public RenderWindow {
	public:

		virtual ~Application();

		// pause & continue draw() call
		void PauseDrawing();
		void StartDrawing();

		/// <summary>
		/// Calls Draw() surrounded with
		/// BeginDraw() and EndDraw()
		/// </summary>
		void Redraw();

		/* exits the application (calls close() on the Window base) */
		void Exit();

		/* call this function to start the sketch */
		template<class TDerived, typename ... TArgs, typename = std::enable_if_t<std::is_base_of_v<Application, TDerived>>>
		static void Launch(const TArgs & ... arguments)
		{
			// We're initializing the variables which needs to be alive ouside
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
		explicit Application(i32_t width = 200, i32_t height = 200, const std::string & title = "Core - Application", RendererType type = RendererType::WindowRenderer);

		/// <summary>
		/// This method gets called
		/// after every Draw() call
		/// </summary>
		virtual void OnFrameProcessed() {}

		/// <summary>
		/// Gets called once
		/// </summary>
		virtual void Setup() {}

		/// <summary>
		/// Gets called every frame
		/// in a loop
		/// </summary>
		virtual void Draw() {}


	private:

		/// <summary>
		/// Calls Setup() surrounded with
		/// BeginDraw() and EndDraw()
		/// </summary>
		void SetupImpl();

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

		/* indicates wether the drawImpl() function gets called or not */
		bool drawingPaused;

		/* instance of a Renderer */
		Renderer * renderer;

		/* instance of a RenderStateManager */
		RenderStateManager * rsm;

	};

}