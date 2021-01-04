#include <Core/Application.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <iostream>

namespace Core
{

	Application::Application(i32_t width, i32_t height, const std::string & title) :
		Window(),
		RenderTarget(this->gctx),
		drawingPaused(false)
	{
		this->create(width, height, title);
		this->setResizable(false);
		this->setMaximizable(false);
		this->setMinimizable(false);
	}

	Application::~Application()
	{
	}

	void Application::pauseDrawing()
	{
		this->drawingPaused = true;
	}

	void Application::startDrawing()
	{
		this->drawingPaused = false;
	}

	void Application::exit()
	{
		this->close();
	}
	
	void Application::setupImpl()
	{
		this->gctx->beginDraw();
		this->setup();
		this->gctx->endDraw();
	}

	void Application::drawImpl()
	{
		this->gctx->beginDraw();
		this->draw();
		this->gctx->endDraw();
	}

	void Application::startSketch()
	{
		/* create the GraphicsContxt */
		this->gctx = new GraphicsContext(this->windowHandle);
		
		/* push the first RenderState onto the stack */
		this->push();

		/* set the framerate limit */
		this->setFramerateLimit(60);
		
		/* setup */
		this->setupImpl();
		
		/* start calling the drawImpl function */
		this->startDrawing();

		while (this->isOpen())
		{
			/* call draw surrounded by begin/end-Draw() */
			if (!this->drawingPaused)
				this->drawImpl();

			/* calculate & limit fps */
			this->handleFps();

			/* raise processEvents() function */
			this->dispatchEvents();
		}

		/* destroy the graphics context */
		delete this->gctx;
		this->gctx = nullptr;
		
		/* destroy the window */
		this->destroy();
	}

}