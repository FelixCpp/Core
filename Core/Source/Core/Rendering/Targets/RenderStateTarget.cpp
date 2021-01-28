#include <Core/Rendering/Targets/RenderStateTarget.hpp>
#include <Core/Rendering/RenderStateManager.hpp>

namespace Core
{
	RenderStateTarget::RenderStateTarget(LateRef<RenderStateManager> rsm) :
		rsm(rsm)
	{
	}

	void RenderStateTarget::Push()
	{
		this->rsm->PushState();
	}

	void RenderStateTarget::Pop()
	{
		this->rsm->PopState();
	}

}