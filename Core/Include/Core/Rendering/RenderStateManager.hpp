#pragma once

#include <stack>
#include <Core/Rendering/RenderState.hpp>

namespace Core
{

	class Renderer;

	class RenderStateManager {
	public:

		/* constructor */
		explicit RenderStateManager(Renderer *& renderer);
		
		/* pushes a RenderState onto the stack and actives the next */
		void PushState();
		
		/* pops a RenderState from the stack and activates the next */
		void PopState();

		/* pops all states from the stack and activates the RenderState */
		void Reset();

		/*
			if the stack of RenderStates is empty,
			the defaultState will be returned.
			Otherwise it will return the highest element
			of the stack
		*/
		RenderState & GetActiveState();

	private:

		/* the lowest state. This RenderState will be active if the stack is empty */
		RenderState defaultState;

		/* the currently used RenderState */
		RenderState * activeState;

		/* a stack of RenderStates */
		std::stack<RenderState> states;

		/* an instance of a GraphicsContext. We need this to push another RenderState onto the stack */
		Renderer *& renderer;

	};

}