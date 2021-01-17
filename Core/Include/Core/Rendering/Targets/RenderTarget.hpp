#pragma once

#include <Core/Rendering/Color.hpp>

#include <Core/Rendering/Targets/ImageRenderTarget.hpp>
#include <Core/Rendering/Targets/ShapeRenderTarget.hpp>
#include <Core/Rendering/Targets/StrokeStyleTarget.hpp>
#include <Core/Rendering/Targets/RenderStateTarget.hpp>
#include <Core/Rendering/Targets/TextRenderTarget.hpp>
#include <Core/Rendering/Targets/PrimitiveRenderTarget.hpp>
#include <Core/Rendering/Targets/TransformationTarget.hpp>
#include <Core/Rendering/Targets/BrushTarget.hpp>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class GraphicsContext;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// Rendering anything
	/// </summary>
	class RenderTarget :
		public ImageRenderTarget,
		public ShapeRenderTarget,
		public StrokeStyleTarget,
		public RenderStateTarget,
		public TransformationTarget,
		public BrushTarget,
		public TextRenderTarget,
		public PrimitiveRenderTarget
	{
	public:

		/// <summary>
		/// passes the parameters to the base classes and
		/// copies them into the member variable "gctx".
		/// </summary>
		/// <param name="gctx">the GraphicsContext</param>
		/// <param name="rsm">the RenderStateManger</param>
		explicit RenderTarget(GraphicsContext *& gctx, RenderStateManager *& rsm);
		
		/// <summary>
		/// A virtual destructor
		/// </summary>
		virtual ~RenderTarget() = default;

		/// <summary>
		/// Clears/Erases the background to
		/// a flat color
		/// </summary>
		/// <param name="color"></param>
		void Background(const Color & color);

	private:

		/// <summary>
		/// An instance of the GraphicsContext
		/// to get access to the RenderTarget
		/// used to clear the background
		/// </summary>
		GraphicsContext *& gctx;

	};

}