#include <Core/Rendering/FrameSaver.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>

namespace Core
{
	
    FrameSaver::FrameSaver(Renderer *& renderer) :
        renderer(renderer)
    { }

	void FrameSaver::SaveFrame(const std::string & filepath)
	{
        this->renderer->SaveFrame(filepath);
	}

} // namespace Core