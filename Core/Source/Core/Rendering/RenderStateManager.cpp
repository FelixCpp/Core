#include <Core/Rendering/RenderStateManager.hpp>

namespace Core
{

	RenderStateManager::RenderStateManager(LateRef<Renderer> renderer) :
		defaultState(renderer),
		activeState(&this->defaultState),
		states(),
		renderer(renderer)
	{
	}

	void RenderStateManager::PushState()
	{
		this->states.push(RenderState(this->renderer));
		this->activeState = &this->states.top(); // we know there is a RenderState available, so use it
		this->GetActiveState().ActivateMatrix();
	}

	void RenderStateManager::PopState()
	{
		/* we only want to pop an element if there is a state on the stack */
		if (!this->states.empty())
		{
			/* pop the state */
			this->states.pop();
		}

		this->activeState = this->states.empty() ? &this->defaultState : &this->states.top();
		this->GetActiveState().ActivateMatrix();
	}

	void RenderStateManager::Reset()
	{
		/* simply clear the stack */
		while (!this->states.empty())
			this->states.pop();

		this->defaultState.Reset(); // reset the matrix before activating it
		this->activeState = &this->defaultState; // since the stack is definitly empty, we use the defaultState
		this->GetActiveState().ActivateMatrix();
	}

	RenderState & RenderStateManager::GetActiveState()
	{
		return *this->activeState;
	}

}