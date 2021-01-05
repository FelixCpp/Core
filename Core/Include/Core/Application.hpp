#pragma once

#include <Core/System/Datatypes.hpp>

#include <Core/Rendering/RenderTarget.hpp>
#include <Core/Rendering/Image.hpp>

#include <Core/Window/Window.hpp>

#include <type_traits>
#include <string>

namespace Core
{

	class GraphicsContext;

	class Application : public Window, public RenderTarget {
	public:

		virtual ~Application() = default;

		virtual void setup() {}
		virtual void draw() {}

		// pause & continue draw() call
		void pauseDrawing();
		void startDrawing();

		/* exits the application (calls close() on the Window base) */
		void exit();

		/* call this function to start the sketch */
		template<class TDerived, typename ... TArgs, typename = std::enable_if_t<std::is_base_of_v<Application, TDerived>>>
		static void launch(const TArgs & ... arguments)
		{
			TDerived instance(std::forward<decltype(arguments)>(arguments)...);
			instance.startSketch();
		}

	protected:

		/* create an Application with the given dimensions as window size */
		explicit Application(i32_t width = 200, i32_t height = 200, const std::string & title = "Core - Application");

		/* resizes the viewport of the internal GraphicsContext */
		void resizeViewport();

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