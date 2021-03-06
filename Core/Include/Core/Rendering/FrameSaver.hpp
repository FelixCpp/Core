#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>
#include <Core/System/LateRef.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <string> // std::string

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
		explicit FrameSaver(LateRef<Renderer> renderer, LateRef<Windowhandle> windowHandle);

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~FrameSaver() = default;

		/// <summary>
		/// Calls SaveFrameAsync.
		/// This method is async.
		/// </summary>
		bool SaveFrame(const std::string & filepath);

	private:

		/// <summary>
		/// Actually takes a screenshot and saves it to
		/// a file.
		/// </summary>
		/// <param name="filepath"></param>
		/// <returns></returns>
		bool SaveFrameImpl(const std::string & filepath);

	private:

		/// <summary>
		/// The renderer
		/// </summary>
		LateRef<Renderer> renderer;

		/// <summary>
		/// The windowhandle.
		/// It's important to keep a LateRef
		/// on it, since it could be recreated
		/// </summary>
		LateRef<Windowhandle> handle;

	};

} // namespace Core