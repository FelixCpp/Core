#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <string>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class Renderer;

	/// <summary>
	/// The FrameSaver class
	/// contains functionality
	/// for taking a screenshot
	/// of the window and save
	/// it to the screen
	/// </summary>
	class FrameSaver {
	public:

		/// <summary>
		/// Takes in the renderer
		/// </summary>
		/// <param name="renderer">The renderer</param>
		explicit FrameSaver(Renderer *& renderer);

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~FrameSaver() = default;

		/// <summary>
		/// Takes a screen shot and
		/// saves it
		/// </summary>
		void SaveFrame(const std::string & filepath);

	private:

		/// <summary>
		/// The renderer
		/// </summary>
		Renderer *& renderer;

	};

} // namespace Core