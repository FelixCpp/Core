#include <Core/Rendering/RenderTarget.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <Core/Window/Window.hpp>

namespace Core
{

	RenderTarget::RenderTarget(GraphicsContext *& gctx) :
		gctx(gctx)
	{
	}

	//Image RenderTarget::loadImage(const std::string & filepath)
	//{
	//	Image image;
	//	image.loadFromFile(filepath, this->gctx);
	//	return image;
	//}
	//
	//Image RenderTarget::createImage(u32_t width, u32_t height, color_t color, i32_t channels, i32_t opacity, ImageInterpolationMode mode)
	//{
	//	Image image;
	//	image.create(width, height, color, channels, opacity, mode, this->gctx);
	//	return image;
	//}

	void RenderTarget::push()
	{
		this->gctx->states.push(RenderState(this->gctx));
		this->fill(Color::White);
		this->stroke(Color::White);
		this->pushMatrix();
	}

	void RenderTarget::pop()
	{
		std::stack<RenderState> & states = this->gctx->states;
		if (states.size() > 1)
		{
			states.pop();
			this->gctx->hwndRenderTarget->SetTransform(states.top().metrics.top());
		}
	}

	void RenderTarget::resetMatrix()
	{
		std::stack<D2D1::Matrix3x2F> & metrics = this->gctx->getActiveRenderState().metrics;
		metrics.top() = D2D1::Matrix3x2F::Identity();
		this->gctx->hwndRenderTarget->SetTransform(metrics.top());
	}

	void RenderTarget::pushMatrix()
	{
		std::stack<D2D1::Matrix3x2F> & metrics = this->gctx->getActiveRenderState().metrics;

		if (metrics.empty())
		{
			metrics.push(D2D1::Matrix3x2F::Identity());
		} else
		{
			metrics.push(metrics.top());
		}

		this->gctx->hwndRenderTarget->SetTransform(metrics.top());
	}

	void RenderTarget::popMatrix()
	{
		std::stack<D2D1::Matrix3x2F> & metrics = this->gctx->getActiveRenderState().metrics;
		if (metrics.size() > 1)
		{
			metrics.pop();
			this->gctx->hwndRenderTarget->SetTransform(metrics.top());
		}
	}

	void RenderTarget::translate(float x, float y)
	{
		D2D1::Matrix3x2F & matrix = this->gctx->getActiveRenderState().metrics.top();
		matrix = matrix * D2D1::Matrix3x2F::Translation(x, y);
		this->gctx->hwndRenderTarget->SetTransform(matrix);
	}

	void RenderTarget::rotate(float degrees)
	{
		D2D1::Matrix3x2F & matrix = this->gctx->getActiveRenderState().metrics.top();
		matrix = matrix * D2D1::Matrix3x2F::Rotation(degrees, D2D1::Point2F(matrix.dx, matrix.dy));
		this->gctx->hwndRenderTarget->SetTransform(matrix);
	}

	void RenderTarget::scale(float factorX, float factorY)
	{
		D2D1::Matrix3x2F & matrix = this->gctx->getActiveRenderState().metrics.top();
		matrix = matrix * D2D1::Matrix3x2F::Scale(factorX, factorY, D2D1::Point2F(matrix.dx, matrix.dy));
		this->gctx->hwndRenderTarget->SetTransform(matrix);
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
		this->gctx->getActiveRenderState().strokeWeight = strokeWeight;
	}

	void RenderTarget::noFill()
	{
		this->gctx->getActiveRenderState().active.fill = nullptr;
	}

	void RenderTarget::noStroke()
	{
		this->gctx->getActiveRenderState().active.stroke = nullptr;
	}

	void RenderTarget::fill(const Color & color)
	{
		RenderState & state = this->gctx->getActiveRenderState();
		state.solid.fill.setColor(color);
		state.active.fill = state.solid.fill.getBrush();
	}

	void RenderTarget::stroke(const Color & color)
	{
		RenderState & state = this->gctx->getActiveRenderState();
		state.solid.stroke.setColor(color);
		state.active.stroke = state.solid.stroke.getBrush();
	}

	void RenderTarget::linearFill(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->gctx->getActiveRenderState();
		state.linear.fill.setStart(startX, startY);
		state.linear.fill.setEnd(endX, endY);
		state.linear.fill.setColors(colors);
		state.active.fill = state.linear.fill.getBrush();
	}

	void RenderTarget::linearStroke(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->gctx->getActiveRenderState();
		state.linear.stroke.setStart(startX, startY);
		state.linear.stroke.setEnd(endX, endY);
		state.linear.stroke.setColors(colors);
		state.active.stroke = state.linear.stroke.getBrush();
	}

	void RenderTarget::radialFill(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->gctx->getActiveRenderState();
		state.radial.fill.setCenter(centerX, centerY);
		state.radial.fill.setRadius(radiusX, radiusY);
		state.radial.fill.setOffset(offsetX, offsetY);
		state.radial.fill.setColors(colors);
		state.active.fill = state.radial.fill.getBrush();
	}

	void RenderTarget::radialStroke(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->gctx->getActiveRenderState();
		state.radial.stroke.setCenter(centerX, centerY);
		state.radial.stroke.setRadius(radiusX, radiusY);
		state.radial.stroke.setOffset(offsetX, offsetY);
		state.radial.stroke.setColors(colors);
		state.active.stroke = state.radial.stroke.getBrush();
	}

	void RenderTarget::rectMode(DrawMode mode)
	{
		this->gctx->getActiveRenderState().rectMode = mode;
	}

	void RenderTarget::rect(float x1, float y1, float x2, float y2, float cornerRadiusX, float cornerRadiusY)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->gctx->getActiveRenderState();

		D2D1_RECT_F rect = D2D1::RectF();

		switch (state.rectMode)
		{
			case DrawMode::Corner:  rect = D2D1::RectF(x1, y1, x1 + x2, y1 + y2); break;
			case DrawMode::Corners: rect = D2D1::RectF(x1, y1, x2, y2); break;
			case DrawMode::Center:  rect = D2D1::RectF(x1 - x2 / 2.f, y1 - y2 / 2.f, x1 + x2 / 2.f, y1 + y2 / 2.f); break;
			default: break;
		}

		const D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, cornerRadiusX, cornerRadiusY);

		if (ID2D1Brush * fill = state.active.fill)
		{
			rt->FillRoundedRectangle(roundedRect, fill);
		}

		if (ID2D1Brush * stroke = state.active.stroke)
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
		this->gctx->getActiveRenderState().ellipseMode = mode;
	}

	void RenderTarget::ellipse(float x, float y, float rx, float ry)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->gctx->getActiveRenderState();

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

		if (ID2D1Brush * fill = state.active.fill)
		{
			rt->FillEllipse(ellipse, fill);
		}

		if (ID2D1Brush * stroke = state.active.stroke)
		{
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
		RenderState & state = this->gctx->getActiveRenderState();
		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), state.strokeWeight, state.strokeWeight);

		if (ID2D1Brush * stroke = state.active.stroke)
		{
			rt->FillEllipse(ellipse, stroke);
		}
	}

	void RenderTarget::line(float x1, float y1, float x2, float y2)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->gctx->getActiveRenderState();

		if (ID2D1Brush * stroke = state.active.stroke)
		{
			rt->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), stroke, state.strokeWeight, state.strokeStyle.getStrokeStyle());
		}
	}

	void RenderTarget::imageMode(DrawMode mode)
	{
		this->gctx->getActiveRenderState().imageMode = mode;
	}

	//void RenderTarget::image(const Core::Image & img, float x, float y)
	//{
	//	image(img, x, y, (float)img.width, (float)img.height);
	//}
	//
	//void RenderTarget::image(const Core::Image & img, float x1, float y1, float x2, float y2)
	//{
	//	if (ID2D1Bitmap * bitmap = img.getBitmap())
	//	{
	//		RenderState & state = this->gctx->getActiveRenderState();
	//
	//		D2D1_RECT_F rect;
	//		switch (state.imageMode)
	//		{
	//			case DrawMode::Corner:  rect = D2D1::RectF(x1, y1, x1 + x2, y1 + y2); break;
	//			case DrawMode::Corners: rect = D2D1::RectF(x1, y1, x2, y2); break;
	//			case DrawMode::Center:  rect = D2D1::RectF(x1 - x2 / 2.f, y1 - y2 / 2.f, x1 + x2 / 2.f, y1 + y2 / 2.f); break;
	//			default: break;
	//		}
	//
	//		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
	//		rt->DrawBitmap(
	//			bitmap,
	//			rect,
	//			img.opacity / 255.f,
	//			static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(img.mode),
	//			D2D1::RectF(0.f, 0.f, (float)img.width, (float)img.height)
	//		);
	//	}
	//}

	void RenderTarget::beginShape()
	{
		this->gctx->getActiveRenderState().shapeRenderer.beginShape();
	}

	void RenderTarget::endShape(ShapeEndType type)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		RenderState & state = this->gctx->getActiveRenderState();
		ShapeRenderer & shapeRenderer = state.shapeRenderer;
		shapeRenderer.endShape(type);

		if (ID2D1Brush * fill = state.active.fill)
		{
			rt->FillGeometry(shapeRenderer.getGeometry(), fill);
		}

		if (ID2D1Brush * stroke = state.active.stroke)
		{
			rt->DrawGeometry(shapeRenderer.getGeometry(), stroke, state.strokeWeight, state.strokeStyle.getStrokeStyle());
		}
	}

	void RenderTarget::shapeFillMode(FillMode fillMode)
	{
		this->gctx->getActiveRenderState().shapeRenderer.setFillMode(fillMode);
	}

	void RenderTarget::shapePathSegment(PathSegment pathSegment)
	{
		this->gctx->getActiveRenderState().shapeRenderer.setPathSegment(pathSegment);
	}

	void RenderTarget::vertex(float x, float y)
	{
		this->gctx->getActiveRenderState().shapeRenderer.vertex(x, y);
	}

	void RenderTarget::quadraticBezier(float x1, float y1, float x2, float y2)
	{
		this->gctx->getActiveRenderState().shapeRenderer.quadraticBezier(x1, y1, x2, y2);
	}

	void RenderTarget::bezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		this->gctx->getActiveRenderState().shapeRenderer.bezier(x1, y1, x2, y2, x3, y3);
	}

	void RenderTarget::arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection sweepDirection, ArcSize size)
	{
		this->gctx->getActiveRenderState().shapeRenderer.arc(x, y, width, height, rotationInDegrees, sweepDirection, size);
	}

	void RenderTarget::strokeStartCap(CapStyle style)
	{
		this->gctx->getActiveRenderState().strokeStyle.setStartCap(style);
	}

	void RenderTarget::strokeEndCap(CapStyle style)
	{
		this->gctx->getActiveRenderState().strokeStyle.setEndCap(style);
	}

	void RenderTarget::strokeDashCap(CapStyle style)
	{
		this->gctx->getActiveRenderState().strokeStyle.setDashCap(style);
	}

	void RenderTarget::strokeLineJoin(LineJoin lineJoin)
	{
		this->gctx->getActiveRenderState().strokeStyle.setLineJoin(lineJoin);
	}

	void RenderTarget::strokeDashStyle(DashStyle style)
	{
		this->gctx->getActiveRenderState().strokeStyle.setDashStyle(style);
	}

	void RenderTarget::strokeDashes(const std::vector<float> & dashes)
	{
		this->gctx->getActiveRenderState().strokeStyle.setDashes(dashes);
	}

	void RenderTarget::strokeDashOffset(float offset)
	{
		this->gctx->getActiveRenderState().strokeStyle.setDashOffset(offset);
	}

	void RenderTarget::strokeMiterLimit(float limit)
	{
		this->gctx->getActiveRenderState().strokeStyle.setMiterLimit(limit);
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
		RenderState & state = this->gctx->getActiveRenderState();
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();

		TextRenderer & textRenderer = state.textRenderer;
		textRenderer.setText(string);
		textRenderer.setMaxWidth(width);
		textRenderer.setMaxHeight(height);

		IDWriteTextLayout * layout = textRenderer.getTextLayout();
		if (!layout) return;

		if (ID2D1Brush * brush = state.active.fill)
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
		this->gctx->getActiveRenderState().textRenderer.setTextSize(textSize);
	}

	void RenderTarget::textUnderline(bool active)
	{
		this->gctx->getActiveRenderState().textRenderer.setUnderline(active);
	}

	void RenderTarget::textStrikeThrough(bool active)
	{
		this->gctx->getActiveRenderState().textRenderer.setStrikeThrough(active);
	}

	void RenderTarget::textFont(const std::string & font)
	{
		this->gctx->getActiveRenderState().textRenderer.setFont(font);
	}

	void RenderTarget::textFontWeight(FontWeight fontWeight)
	{
		this->gctx->getActiveRenderState().textRenderer.setFontWeight(fontWeight);
	}

	void RenderTarget::textFontStyle(FontStyle fontStyle)
	{
		this->gctx->getActiveRenderState().textRenderer.setFontStyle(fontStyle);
	}

	void RenderTarget::textFontStretch(FontStretch fontStretch)
	{
		this->gctx->getActiveRenderState().textRenderer.setFontStretch(fontStretch);
	}

	void RenderTarget::textWordWrapping(WordWrapping wordWrapping)
	{
		this->gctx->getActiveRenderState().textRenderer.setWordWrapping(wordWrapping);
	}

	void RenderTarget::textAlignment(Core::TextAlignment textAlignment)
	{
		this->gctx->getActiveRenderState().textRenderer.setTextAlignment(textAlignment);
	}

	void RenderTarget::textParagraphAlignment(ParagraphAlignment paragraphAlignment)
	{
		this->gctx->getActiveRenderState().textRenderer.setParagraphAlignment(paragraphAlignment);
	}

	FVector2 RenderTarget::getTextSize()
	{
		return this->gctx->getActiveRenderState().textRenderer.getSize();
	}

	FVector2 RenderTarget::getTextSize(const std::string & content)
	{
		this->gctx->getActiveRenderState().textRenderer.setText(content);
		return getTextSize();
	}

	GraphicsContext *& RenderTarget::getGraphicsContext() const
	{
		return this->gctx;
	}

}