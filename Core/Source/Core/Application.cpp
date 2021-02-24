#include <Core/Application.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/FactoryManager.hpp>
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

	void Application::Redraw()
	{
		this->BeginAnimationFrame();
		this->Draw();
		this->EndAnimationFrame();
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
		this->BeginAnimationFrame();
		this->Setup();
		this->EndAnimationFrame();
	}

	void Application::StartSketch()
	{
		// Initialize the graphics
		if (!this->renderer->Initialize(this->windowHandle))
		{
			this->Close();
			return;
		}

		// set the framerate limit
		this->SetFramerateLimit(60);

		// start calling the DrawImpl function
		this->StartDrawing();

		// Setup
		this->SetupImpl();

		while (this->IsOpen())
		{
			// calculate & limit fps
			this->HandleFps();

			// call draw surrounded by begin/end-Draw()
			if (!this->drawingPaused)
			{
				this->Redraw();
				this->OnFrameProcessed();
			}

			// pops every renderstate and activates the default
			this->rsm->Reset();
			this->DispatchEvents();
		}

		// Destroys the graphics
		this->renderer->Destroy();
	}

	void Application::GlobalInit()
	{
		FactoryManager::Initialize();
	}

	void Application::GlobalDestroy()
	{
		FactoryManager::Destroy();
	}

}