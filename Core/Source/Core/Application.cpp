#include <Core/Application.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/FactoryManager.hpp>
#include <Core/Rendering/Renderers/RendererFactory.hpp>

namespace Core
{

	void LoopManager::StartDrawing()
	{
		this->drawingActive = true;
	}

	void LoopManager::PauseDrawing()
	{
		this->drawingActive = false;
	}

	void LoopManager::Redraw()
	{
		this->renderer->BeginDraw();
		this->Draw();
		this->renderer->BeginDraw();
		
		this->OnFrameProcessed();
	}

	bool LoopManager::CanDraw() const
	{
		return this->drawingActive;
	}

	LoopManager::LoopManager(LateRef<Renderer> renderer) :
		drawingActive(false),
		renderer(renderer)
	{ }

	Application::~Application()
	{
		delete this->renderer;
		this->renderer = nullptr;

		delete this->rsm;
		this->rsm = nullptr;
	}

	void Application::Exit()
	{
		this->Close();
	}
	
	Application::Application(Int32 width, Int32 height, const std::string & title, RendererType type) :
		RenderWindow(this->renderer, this->rsm),
		LoopManager(this->renderer),
		renderer(RendererFactory::Create(type)),
		rsm(new RenderStateManager(this->renderer))
	{
		this->Create(width, height, title);
		this->SetResizable(false);
		this->SetMaximizable(false);
		this->SetMinimizable(false);
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

		// Call Setup
		this->renderer->BeginDraw();
		this->Setup();
		this->renderer->EndDraw();

		while (this->IsOpen())
		{
			// calculate & limit fps
			this->HandleFps();

			if (this->CanDraw())
				this->Redraw();

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