#pragma once

#include <Core/System/Datatypes.hpp>

#include <Core/Rendering/RenderWindow.hpp>

#include <type_traits>
#include <string>

namespace Core
{

	class GraphicsContext;
	class RenderStateManager;

	class Application : public RenderWindow {
	public:

		virtual ~Application();

		virtual void Setup() {}
		virtual void Draw() {}

		// pause & continue draw() call
		void PauseDrawing();
		void StartDrawing();

		/* exits the application (calls close() on the Window base) */
		void Exit();

		/* call this function to start the sketch */
		template<class TDerived, typename ... TArgs, typename = std::enable_if_t<std::is_base_of_v<Application, TDerived>>>
		static void Launch(const TArgs & ... arguments)
		{
			TDerived instance(std::forward<decltype(arguments)>(arguments)...);
			instance.startSketch();
		}

	protected:

		/* create an Application with the given dimensions as window size */
		explicit Application(i32_t width = 200, i32_t height = 200, const std::string & title = "Core - Application");

	private:

		void SetupImpl();
		void DrawImpl();

		void StartSketch();

	private:

		/* since we have raw pointers we don't want anyone to copy this class */
		Application(const Application &) = delete;
		Application(Application &&) = delete;
		Application & operator=(const Application &) = delete;
		Application & operator=(Application &&) = delete;

	private:

		/* indicates wether the drawImpl() function gets called or not */
		bool drawingPaused;

		/* instance of a GraphicsContext */
		GraphicsContext * gctx;

		/* instance of a RenderStateManager */
		RenderStateManager * rsm;

	};

}