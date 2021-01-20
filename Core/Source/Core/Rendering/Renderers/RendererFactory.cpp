#include <Core/Rendering/Renderers/RendererFactory.hpp>

#include <Core/Rendering/Renderers/WindowRenderer.hpp>
#include <Core/Rendering/Renderers/BitmapRenderer.hpp>

namespace Core
{

	RendererBase * RendererFactory::Create(RendererType type)
	{
		switch (type)
		{
			case RendererType::WindowRenderer: return new WindowRenderer();
			case RendererType::BitmapRenderer: return new BitmapRenderer();
			default: return nullptr;
		}
	}

} // namespace Core