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
		this->activeState = &this->states.top(); // we know there is a RenderState available, so use it
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

	void RenderStateManager::reset()
	{
		/* simply clear the stack */
		while (!this->states.empty())
			this->states.pop();

		this->defaultState.reset(); // reset the matrix before activating it
		this->activeState = &this->defaultState; // since the stack is definitly empty, we use the defaultState
		this->getActiveState().activateMatrix();
	}

	RenderState & RenderStateManager::getActiveState()
	{
		return *this->activeState;
	}

}