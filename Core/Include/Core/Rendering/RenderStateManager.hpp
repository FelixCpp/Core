#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/RenderState.hpp>
#include <Core/System/LateRef.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <stack>

namespace Core
{

	class Renderer;

	class RenderStateManager {
	public:

		/* constructor */
		explicit RenderStateManager(LateRef<Renderer> renderer);
		
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

		/* an instance of a Renderer. We need this to push another RenderState onto the stack */
		LateRef<Renderer> renderer;

	};

}