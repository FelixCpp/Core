#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/LateRef.hpp> // Core::LateRef

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class RenderStateManager;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// RenderStates
	/// </summary>
	class RenderStateTarget {
	public:

		/// <summary>
		/// constructor which passes the RenderStateManager
		/// to its member variables.
		/// </summary>
		/// <param name="rsm">the RenderStateManager which holds the RenderStates to modify</param>
		explicit RenderStateTarget(LateRef<RenderStateManager> rsm);

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
		/// to get access the RenderStates
		/// </summary>
		LateRef<RenderStateManager> rsm;

	};

	/// <summary>
	/// Example:
	/// <code>
	/// 
	/// ...
	/// float time = 0.f;
	/// 
	/// Fill(Core::Color::LightRed);
	/// Stroke(Core::Color::White);
	/// Quad(100.f, 100.f, 75.f);
	/// 
	/// Push();
	/// Fill(Core::Color::LightBlue);
	/// Translate(width / 2, height / 2);
	/// Rotate(time);
	/// RectMode(Core::DrawMode::Center);
	/// Quad(0.f, 0.f, 100.f);
	/// Pop();
	/// 
	/// NoStroke();
	/// Ellipse(100.f, 200.f, 50.f);
	/// 
	/// time += 0.1f;
	/// ...
	/// 
	/// </code>
	/// </summary>

}