#pragma once

#include <Core/System/Datatypes.hpp>

#include <Core/Application/RenderTarget.hpp>
#include <Core/Application/Image.hpp>

#include <Core/Window/Window.hpp>

#include <string>

namespace Core
{

	class GraphicsContext;

	class Application : public Window, public RenderTarget {
	public:

		virtual ~Application();

		virtual void setup() {}
		virtual void draw() {}

		// pause & continue draw() call
		void pauseDrawing();
		void startDrawing();

		/* exits the application (calls close() on the Window base) */
		void exit();

		/* call this function to start the sketch */
		template<class T, typename ... TArgs>
		static void launch(const TArgs & ... arguments)
		{
			T instance(std::forward<decltype(arguments)>(arguments)...);
			instance.startSketch();
		}

	protected:

		/* create an Application with the given dimensions as window size */
		explicit Application(i32_t width = 200, i32_t height = 200, const std::string & title = "Core - Application");

	private:

		void setupImpl();
		void drawImpl();

		void startSketch();

	private:

		/* graphics context */
		GraphicsContext * gctx;

		/* indicates wether the drawImpl() function gets called or not */
		bool drawingPaused;

	};

}