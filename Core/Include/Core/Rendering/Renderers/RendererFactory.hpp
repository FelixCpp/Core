#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/StaticObject.hpp> // Core::StaticObject
#include <Core/Rendering/Renderers/RendererType.hpp> // Core::RendererType

namespace Core
{

	class RendererBase;

	struct RendererFactory : StaticObject {

		static RendererBase * Create(RendererType type);

	};

} // namespace Core