#pragma once

#include <stack>
#include <Core/Rendering/RenderState.hpp>

namespace Core
{

	class GraphicsContext;

	class RenderStateManager {
	public:

		/* constructor */
		explicit RenderStateManager(GraphicsContext *& gctx);
		
		/* pushes a RenderState onto the stack and actives the next */
		void pushState();
		
		/* pops a RenderState from the stack and activates the next */
		void popState();

		/*
			if the stack of RenderStates is empty,
			the defaultState will be returned.
			Otherwise it will return the highest element
			of the stack
		*/
		RenderState & getActiveState();

	private:

		/* the lowest state. This RenderState will be active if the stack is empty */
		RenderState defaultState;

		/* the currently used RenderState */
		RenderState * activeState;

		/* a stack of RenderStates */
		std::stack<RenderState> states;

		/* an instance of a GraphicsContext. We need this to push another RenderState onto the stack */
		GraphicsContext *& gctx;

	};

}