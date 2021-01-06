#include <Core/Rendering/RenderTarget.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderStateManager.hpp>

#include <Core/Window/Window.hpp>

namespace Core
{

	RenderTarget::RenderTarget(GraphicsContext *& gctx) :
		gctx(gctx),
		rsm(new RenderStateManager(gctx))
	{
	}

	RenderTarget::~RenderTarget()
	{
		delete this->rsm;
		this->rsm = nullptr;
	}

	void RenderTarget::push()
	{
		this->rsm->pushState();
	}

	void RenderTarget::pop()
	{
		this->rsm->popState();
	}

	void RenderTarget::resetMatrix()
	{
		D2D1::Matrix3x2F & matrix = this->rsm->getActiveState().getActiveMatrix();
		this->rsm->getActiveState().setActiveMatrix(D2D1::Matrix3x2F::Identity());
	}

	void RenderTarget::pushMatrix()
	{
		this->rsm->getActiveState().pushMatrix();
	}

	void RenderTarget::popMatrix()
	{
		this->rsm->getActiveState().popMatrix();
	}

	void RenderTarget::translate(float x, float y)
	{
		const D2D1::Matrix3x2F & matrix = this->rsm->getActiveState().getActiveMatrix();
		this->rsm->getActiveState().setActiveMatrix(matrix * D2D1::Matrix3x2F::Translation(x, y));	
	}

	void RenderTarget::rotate(float degrees)
	{
		const D2D1::Matrix3x2F & matrix = this->rsm->getActiveState().getActiveMatrix();
		this->rsm->getActiveState().setActiveMatrix(matrix * D2D1::Matrix3x2F::Rotation(degrees, D2D1::Point2F(matrix.dx, matrix.dy)));
	}

	void RenderTarget::scale(float factorX, float factorY)
	{
		const D2D1::Matrix3x2F & matrix = this->rsm->getActiveState().getActiveMatrix();
		this->rsm->getActiveState().setActiveMatrix(matrix * D2D1::Matrix3x2F::Scale(factorX, factorY, D2D1::Point2F(matrix.dx, matrix.dy)));
	}

	void RenderTarget::background(const Color & color)
	{
		this->gctx->hwndRenderTarget->Clear(D2D1::ColorF(
			(float)color.r / 255.f,
			(float)color.g / 255.f,
			(float)color.b / 255.f,
			1.f
		));
	}

	void RenderTarget::strokeWeight(float strokeWeight)
	{
		this->rsm->getActiveState().strokeWeight = strokeWeight;
	}

	void RenderTarget::noFill()
	{
		this->rsm->getActiveState().activeFill = nullptr;
	}

	void RenderTarget::noStroke()
	{
		this->rsm->getActiveState().activeStroke = nullptr;
	}

	void RenderTarget::fill(const Color & color)
	{
		RenderState & state = this->rsm->getActiveState();
		state.solidFill.setColor(color);
		state.activeFill = state.solidFill.getBrush();
	}

	void RenderTarget::stroke(const Color & color)
	{
		RenderState & state = this->rsm->getActiveState();
		state.solidStroke.setColor(color);
		state.activeStroke = state.solidStroke.getBrush();
	}

	void RenderTarget::linearFill(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->rsm->getActiveState();
		state.linearFill.setStart(startX, startY);
		state.linearFill.setEnd(endX, endY);
		state.linearFill.setColors(colors);
		state.activeFill = state.linearFill.getBrush();
	}

	void RenderTarget::linearStroke(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->rsm->getActiveState();
		state.linearStroke.setStart(startX, startY);
		state.linearStroke.setEnd(endX, endY);
		state.linearStroke.setColors(colors);
		state.activeStroke = state.linearStroke.getBrush();
	}

	void RenderTarget::radialFill(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->rsm->getActiveState();
		state.radialFill.setCenter(centerX, centerY);
		state.radialFill.setRadius(radiusX, radiusY);
		state.radialFill.setOffset(offsetX, offsetY);
		state.radialFill.setColors(colors);
		state.activeFill = state.radialFill.getBrush();
	}

	void RenderTarget::radialStroke(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->rsm->getActiveState();
		state.radialStroke.setCenter(centerX, centerY);
		state.radialStroke.setRadius(radiusX, radiusY);
		state.radialStroke.setOffset(offsetX, offsetY);
		state.radialStroke.setColors(colors);
		state.activeStroke = state.radialStroke.getBrush();
	}

	void RenderTarget::rectMode(DrawMode mode)
	{
		this->rsm->getActiveState().rectMode = mode;
	}

	void RenderTarget::rect(float x1, float y1, float x2, float y2, float cornerRadiusX, float cornerRadiusY)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->rsm->getActiveState();

		D2D1_RECT_F rect = D2D1::RectF();

		switch (state.rectMode)
		{
			case DrawMode::Corner:  rect = D2D1::RectF(x1, y1, x1 + x2, y1 + y2); break;
			case DrawMode::Corners: rect = D2D1::RectF(x1, y1, x2, y2); break;
			case DrawMode::Center:  rect = D2D1::RectF(x1 - x2 / 2.f, y1 - y2 / 2.f, x1 + x2 / 2.f, y1 + y2 / 2.f); break;
			default: break;
		}

		const D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, cornerRadiusX, cornerRadiusY);

		if (ID2D1Brush * fill = state.activeFill)
		{
			rt->FillRoundedRectangle(roundedRect, fill);
		}

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->DrawRoundedRectangle(roundedRect, stroke, state.strokeWeight, state.strokeStyle.getStrokeStyle());
		}
	}

	void RenderTarget::quad(float x, float y, float size, float cornerRadiusX, float cornerRadiusY)
	{
		rect(x, y, size, size, cornerRadiusX, cornerRadiusY);
	}

	void RenderTarget::ellipseMode(DrawMode mode)
	{
		this->rsm->getActiveState().ellipseMode = mode;
	}

	void RenderTarget::ellipse(float x, float y, float rx, float ry)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->rsm->getActiveState();

		D2D1_ELLIPSE ellipse;

		switch (state.ellipseMode)
		{
			case DrawMode::Corner:  ellipse = D2D1::Ellipse(D2D1::Point2F(x + rx, y + ry), rx, ry); break;
			case DrawMode::Corners:
			{
				float w = (rx - x) / 2.f;
				float h = (ry - y) / 2.f;
				float xpos = x + w;
				float ypos = y + h;
				ellipse = D2D1::Ellipse(D2D1::Point2F(xpos, ypos), w, h);
			} break;
			case DrawMode::Center:  ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry); break;
			default: break;
		}

		if (ID2D1Brush * fill = state.activeFill)
		{
			rt->FillEllipse(ellipse, fill);
		}

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			std::printf("STROKE ACTIVE\n");
			rt->DrawEllipse(ellipse, stroke, state.strokeWeight, state.strokeStyle.getStrokeStyle());
		}
	}

	void RenderTarget::circle(float x, float y, float radius)
	{
		ellipse(x, y, radius, radius);
	}

	void RenderTarget::point(float x, float y)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->rsm->getActiveState();
		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), state.strokeWeight, state.strokeWeight);

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->FillEllipse(ellipse, stroke);
		}
	}

	void RenderTarget::line(float x1, float y1, float x2, float y2)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->rsm->getActiveState();

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), stroke, state.strokeWeight, state.strokeStyle.getStrokeStyle());
		}
	}

	void RenderTarget::imageMode(DrawMode mode)
	{
		this->rsm->getActiveState().imageMode = mode;
	}

	void RenderTarget::beginShape()
	{
		this->rsm->getActiveState().shapeRenderer.beginShape();
	}

	void RenderTarget::endShape(ShapeEndType type)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->rsm->getActiveState();
		ShapeRenderer & shapeRenderer = state.shapeRenderer;
		shapeRenderer.endShape(type);

		if (ID2D1Brush * fill = state.activeFill)
		{
			rt->FillGeometry(shapeRenderer.getGeometry(), fill);
		}

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->DrawGeometry(shapeRenderer.getGeometry(), stroke, state.strokeWeight, state.strokeStyle.getStrokeStyle());
		}
	}

	void RenderTarget::shapeFillMode(FillMode fillMode)
	{
		this->rsm->getActiveState().shapeRenderer.setFillMode(fillMode);
	}

	void RenderTarget::shapePathSegment(PathSegment pathSegment)
	{
		this->rsm->getActiveState().shapeRenderer.setPathSegment(pathSegment);
	}

	void RenderTarget::vertex(float x, float y)
	{
		this->rsm->getActiveState().shapeRenderer.vertex(x, y);
	}

	void RenderTarget::quadraticBezier(float x1, float y1, float x2, float y2)
	{
		this->rsm->getActiveState().shapeRenderer.quadraticBezier(x1, y1, x2, y2);
	}

	void RenderTarget::bezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		this->rsm->getActiveState().shapeRenderer.bezier(x1, y1, x2, y2, x3, y3);
	}

	void RenderTarget::arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection sweepDirection, ArcSize size)
	{
		this->rsm->getActiveState().shapeRenderer.arc(x, y, width, height, rotationInDegrees, sweepDirection, size);
	}

	void RenderTarget::strokeStartCap(CapStyle style)
	{
		this->rsm->getActiveState().strokeStyle.setStartCap(style);
	}

	void RenderTarget::strokeEndCap(CapStyle style)
	{
		this->rsm->getActiveState().strokeStyle.setEndCap(style);
	}

	void RenderTarget::strokeDashCap(CapStyle style)
	{
		this->rsm->getActiveState().strokeStyle.setDashCap(style);
	}

	void RenderTarget::strokeLineJoin(LineJoin lineJoin)
	{
		this->rsm->getActiveState().strokeStyle.setLineJoin(lineJoin);
	}

	void RenderTarget::strokeDashStyle(DashStyle style)
	{
		this->rsm->getActiveState().strokeStyle.setDashStyle(style);
	}

	void RenderTarget::strokeDashes(const std::vector<float> & dashes)
	{
		this->rsm->getActiveState().strokeStyle.setDashes(dashes);
	}

	void RenderTarget::strokeDashOffset(float offset)
	{
		this->rsm->getActiveState().strokeStyle.setDashOffset(offset);
	}

	void RenderTarget::strokeMiterLimit(float limit)
	{
		this->rsm->getActiveState().strokeStyle.setMiterLimit(limit);
	}

	void RenderTarget::text(const std::string & string, float x, float y)
	{
		const FVector2 size = getTextSize(string);
		text(string, x, y, size.width, size.height);
	}

	void RenderTarget::text(const std::string & string, float x, float y, float width)
	{
		const FVector2 size = getTextSize(string);
		text(string, x, y, width, size.height);
	}

	void RenderTarget::text(const std::string & string, float x, float y, float width, float height)
	{
		RenderState & state = this->rsm->getActiveState();
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();

		TextRenderer & textRenderer = state.textRenderer;
		textRenderer.setText(string);
		textRenderer.setMaxWidth(width);
		textRenderer.setMaxHeight(height);

		IDWriteTextLayout * layout = textRenderer.getTextLayout();
		if (!layout) return;

		if (ID2D1Brush * brush = state.activeFill)
		{
			rt->DrawTextLayout(
				D2D1::Point2F(x, y),
				layout,
				brush
			);
		}
	}

	void RenderTarget::textSize(float textSize)
	{
		this->rsm->getActiveState().textRenderer.setTextSize(textSize);
	}

	void RenderTarget::textUnderline(bool active)
	{
		this->rsm->getActiveState().textRenderer.setUnderline(active);
	}

	void RenderTarget::textStrikeThrough(bool active)
	{
		this->rsm->getActiveState().textRenderer.setStrikeThrough(active);
	}

	void RenderTarget::textFont(const std::string & font)
	{
		this->rsm->getActiveState().textRenderer.setFont(font);
	}

	void RenderTarget::textFontWeight(FontWeight fontWeight)
	{
		this->rsm->getActiveState().textRenderer.setFontWeight(fontWeight);
	}

	void RenderTarget::textFontStyle(FontStyle fontStyle)
	{
		this->rsm->getActiveState().textRenderer.setFontStyle(fontStyle);
	}

	void RenderTarget::textFontStretch(FontStretch fontStretch)
	{
		this->rsm->getActiveState().textRenderer.setFontStretch(fontStretch);
	}

	void RenderTarget::textWordWrapping(WordWrapping wordWrapping)
	{
		this->rsm->getActiveState().textRenderer.setWordWrapping(wordWrapping);
	}

	void RenderTarget::textAlignment(Core::TextAlignment textAlignment)
	{
		this->rsm->getActiveState().textRenderer.setTextAlignment(textAlignment);
	}

	void RenderTarget::textParagraphAlignment(ParagraphAlignment paragraphAlignment)
	{
		this->rsm->getActiveState().textRenderer.setParagraphAlignment(paragraphAlignment);
	}

	FVector2 RenderTarget::getTextSize()
	{
		return this->rsm->getActiveState().textRenderer.getSize();
	}

	FVector2 RenderTarget::getTextSize(const std::string & content)
	{
		this->rsm->getActiveState().textRenderer.setText(content);
		return getTextSize();
	}

	GraphicsContext *& RenderTarget::getGraphicsContext() const
	{
		return this->gctx;
	}

}