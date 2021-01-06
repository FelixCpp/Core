#include <Core/Rendering/RenderStateManager.hpp>

namespace Core
{

	RenderStateManager::RenderStateManager(GraphicsContext *& gctx) :
		defaultState(gctx),
		activeState(&this->defaultState),
		states(),
		gctx(gctx)
	{
	}

	void RenderStateManager::pushState()
	{
		this->states.push(RenderState(this->gctx));
		this->activeState = this->states.empty() ? &this->defaultState : &this->states.top();
		this->getActiveState().activateMatrix();
	}

	void RenderStateManager::popState()
	{
		/* we only want to pop an element if there is a state on the stack */
		if (!this->states.empty())
		{
			/* pop the state */
			this->states.pop();
		}

		this->activeState = this->states.empty() ? &this->defaultState : &this->states.top();
		this->getActiveState().activateMatrix();
	}

	RenderState & RenderStateManager::getActiveState()
	{
		return *this->activeState;
	}

}