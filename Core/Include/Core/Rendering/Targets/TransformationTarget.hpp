#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/LateRef.hpp>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class RenderState;
	class RenderStateManager;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// metrics/transformations
	/// </summary>
	class TransformationTarget {
	public:

		/// <summary>
		/// constructor which passes the RenderStateManager
		/// to its member variables.
		/// </summary>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState holding informations about the transformation metrics</param>
		explicit TransformationTarget(LateRef<RenderStateManager> rsm);

		/// <summary>
		/// A virtual destructor
		/// </summary>
		virtual ~TransformationTarget() = default;

		/// <summary>
		/// Resets the current
		/// transformation back
		/// to its initial value
		/// </summary>
		void ResetMatrix();

		/// <summary>
		/// Pushes another matrix onto
		/// the stack of metrics and
		/// activates it.
		/// The new matrix will advance the
		/// matrix activated before pushing a new
		/// one.
		/// </summary>
		void PushMatrix();

		/// <summary>
		/// Pops the current matrix from
		/// the stack of metrics and
		/// activates it.
		/// </summary>
		void PopMatrix();

		/// <summary>
		/// Translates the center to
		/// the given coordinates
		/// </summary>
		/// <param name="x">point x in pixel space</param>
		/// <param name="y">point y in pixel space</param>
		void Translate(float x, float y);

		/// <summary>
		/// Rotates around the center
		/// </summary>
		/// <param name="degrees">rotation angle in degrees</param>
		void Rotate(float angle);

		/// <summary>
		/// Scales the transformation by
		/// the given factor
		/// </summary>
		/// <param name="factorX">width factor</param>
		/// <param name="factorY">height factor</param>
		void Scale(float factorX, float factorY);

		/// <summary>
		/// Shears the matrix by given angles
		/// </summary>
		/// <param name="angleX">angle for left & right side in degrees</param>
		/// <param name="angleY">angle for top & bottom side in degrees</param>
		void Skew(float angleX, float angleY);

	private:

		/// <summary>
		/// returns the current shape of the
		/// active RenderState
		/// </summary>
		RenderState & GetState();

	private:

		/// <summary>
		/// An instance of the RenderStateManager
		/// to get access to the brushes / drawing
		/// properties
		/// </summary>
		LateRef<RenderStateManager> rsm;

	};

	/// <summary>
	/// Example:
	/// <code>
	/// float time = 0.f;
	/// ...
	/// PushMatrix();
	/// Translate(width / 2, height / 2);
	/// Rotate(45.f);
	/// Scale(sinf(time), cosf(time));
	/// RectMode(Core::DrawMode::Center);
	/// Rect(200.f, 200.f, 100.f, 100.f);
	/// PopMatrix();
	/// time += 0.1f;
	/// ...
	/// 
	/// </code>
	/// </summary>

}