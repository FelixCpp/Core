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

	void Application::PauseDrawing()
	{
		this->drawingPaused = true;
	}

	void Application::StartDrawing()
	{
		this->drawingPaused = false;
	}

	void Application::Exit()
	{
		this->Close();
	}
	
	Application::Application(i32_t width, i32_t height, const std::string & title) :
		RenderWindow(this->gctx, this->rsm),
		drawingPaused(true),
		gctx(new GraphicsContext()),
		rsm(new RenderStateManager(this->gctx))
	{
		this->Create(width, height, title);
		this->SetResizable(false);
		this->SetMaximizable(false);
		this->SetMinimizable(false);
	}

	void Application::SetupImpl()
	{
		this->gctx->BeginDraw();
		this->Setup();
		this->gctx->EndDraw();
	}

	void Application::DrawImpl()
	{
		this->gctx->BeginDraw();
		this->Draw();
		this->gctx->EndDraw();
	}

	void Application::StartSketch()
	{
		/* set the framerate limit */
		this->SetFramerateLimit(60);
		
		/* setup */
		this->SetupImpl();
		
		/* start calling the drawImpl function */
		this->StartDrawing();

		while (this->IsOpen())
		{
			/* pops every renderstate and activates the default*/
			this->rsm->Reset();

			/* call draw surrounded by begin/end-Draw() */
			if (!this->drawingPaused)
				this->DrawImpl();

			/* calculate & limit fps */
			this->HandleFps();

			/* raise processEvents() function */
			this->DispatchEvents();
		}
	}

}