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

#include <vector>

namespace Core
{

	class GraphicsContext;

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

		/* RenderTarget constructor */
		explicit RenderTarget(GraphicsContext *& gctx, RenderStateManager *& rsm);
		
		/* virtual destructor */
		virtual ~RenderTarget() = default;

		/* clears/erases the background */
		void Background(const Color & color);

	private:

		/* instance of a GraphicsContext. Used for drawing routines */
		GraphicsContext *& gctx;

		/* instance of a RenderStateManager. */
		RenderStateManager *& rsm;

	};

}