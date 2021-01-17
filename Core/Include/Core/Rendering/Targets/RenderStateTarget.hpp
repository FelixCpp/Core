#pragma once

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class RenderStateManager;

	class RenderStateTarget {
	public:

		/// <summary>
		/// constructor which passes the RenderStateManager
		/// to its member variables.
		/// 
		/// Its needed to get access to the Shape attribute
		/// in the active RenderState
		/// </summary>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState which holds the shape object</param>
		explicit RenderStateTarget(RenderStateManager *& rsm);

		/// <summary>
		/// virtual destructor
		/// </summary>
		virtual ~RenderStateTarget() = default;

		/// <summary>
		/// Pushes another RenderState
		/// onto the stack and activates
		/// it.
		/// Activating a RenderState resets
		/// all of its properties
		/// like fill+stroke brush, strokeWeight
		/// etc.
		/// </summary>
		void Push();

		/// <summary>
		/// Pops the highest RenderState from the stack
		/// and activates the state below.
		/// 
		/// Activating a RenderState resets
		/// all of its properties
		/// like fill+stroke brush, strokeWeight
		/// etc.
		/// </summary>
		void Pop();

	private:

		/// <summary>
		/// An instance of the RenderStateManager
		/// to get access to the brushes / drawing
		/// properties
		/// </summary>
		RenderStateManager *& rsm;

	};

}