#pragma once

#include <Core/Rendering/Helpers.hpp>
#include <Core/Rendering/DrawMode.hpp>
#include <Core/Rendering/TextAttributes.hpp>
#include <Core/Maths/Vector2.hpp>
#include <Core/Rendering/Color.hpp>

#include <Core/Rendering/Targets/ImageTarget.hpp>
#include <Core/Rendering/Targets/ShapeTarget.hpp>
#include <Core/Rendering/Targets/StrokeStyleTarget.hpp>
#include <Core/Rendering/Targets/RenderStateTarget.hpp>
#include <Core/Rendering/Targets/TransformationTarget.hpp>
#include <Core/Rendering/Targets/BrushTarget.hpp>
#include <Core/Rendering/Targets/TextTarget.hpp>
#include <Core/Rendering/Targets/PrimitiveRenderTarget.hpp>

#include <vector>

namespace Core
{

	class GraphicsContext;

	class RenderTarget :
		public ImageTarget,
		public ShapeTarget,
		public StrokeStyleTarget,
		public RenderStateTarget,
		public TransformationTarget,
		public BrushTarget,
		public TextTarget,
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