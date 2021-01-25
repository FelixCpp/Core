#include <Core/Rendering/Renderers/RendererFactory.hpp>

#include <Core/Rendering/Renderers/WindowRenderer.hpp>

namespace Core
{

	Renderer * RendererFactory::Create(RendererType type)
	{
		switch (type)
		{
			case RendererType::WindowRenderer: return new WindowRenderer();
			default: return nullptr;
		}
	}

} // namespace Core