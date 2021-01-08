#include <Core/Application.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderStateManager.hpp>

namespace Core
{
	
	Application::~Application()
	{
		delete this->gctx;
		this->gctx = nullptr;

		delete this->rsm;
		this->rsm = nullptr;
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
	
	Application::Application(i32_t width, i32_t height, const std::string & title) :
		RenderWindow(this->gctx, this->rsm),
		drawingPaused(true),
		gctx(new GraphicsContext()),
		rsm(new RenderStateManager(this->gctx))
	{
		this->create(width, height, title);
		this->setResizable(false);
		this->setMaximizable(false);
		this->setMinimizable(false);
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
		/* set the framerate limit */
		this->setFramerateLimit(60);
		
		/* setup */
		this->setupImpl();
		
		/* start calling the drawImpl function */
		this->startDrawing();

		while (this->isOpen())
		{
			/* pops every renderstate and activates the default*/
			this->rsm->reset();

			/* call draw surrounded by begin/end-Draw() */
			if (!this->drawingPaused)
				this->drawImpl();

			/* calculate & limit fps */
			this->handleFps();

			/* raise processEvents() function */
			this->dispatchEvents();
		}
	}

}