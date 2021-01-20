#include <Core/Application.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/Renderers/RendererFactory.hpp>

namespace Core
{
	
	Application::~Application()
	{
		delete this->renderer;
		this->renderer = nullptr;

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
	
	Application::Application(i32_t width, i32_t height, const std::string & title, RendererType type) :
		RenderWindow(this->renderer, this->rsm),
		drawingPaused(true),
		renderer(RendererFactory::Create(type)),
		rsm(new RenderStateManager(this->renderer))
	{
		this->Create(width, height, title);
		this->SetResizable(false);
		this->SetMaximizable(false);
		this->SetMinimizable(false);
	}

	void Application::SetupImpl()
	{
		this->renderer->BeginDraw();
		this->Setup();
		this->renderer->EndDraw();
	}

	void Application::DrawImpl()
	{
		this->renderer->BeginDraw();
		this->Draw();
		this->renderer->EndDraw();
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
			{
				this->DrawImpl();
				this->OnFrameProcessed();
			}

			/* calculate & limit fps */
			this->HandleFps();

			/* raise processEvents() function */
			this->DispatchEvents();
		}
	}

}