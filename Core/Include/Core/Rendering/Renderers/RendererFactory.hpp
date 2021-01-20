#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/StaticObject.hpp> // Core::StaticObject
#include <Core/Rendering/Renderers/RendererType.hpp> // Core::RendererType

namespace Core
{

	class Renderer;

	struct RendererFactory : StaticObject {

		static Renderer * Create(RendererType type);

	}; // struct RendererFactory

} // namespace Core