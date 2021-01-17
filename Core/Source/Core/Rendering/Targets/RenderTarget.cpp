#include <Core/Rendering/Targets/RenderTarget.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderStateManager.hpp>

#include <Core/Window/Window.hpp>

namespace Core
{

	RenderTarget::RenderTarget(GraphicsContext *& gctx, RenderStateManager *& rsm) :
		ImageTarget(gctx, rsm),
		ShapeTarget(gctx, rsm),
		StrokeStyleTarget(rsm),
		RenderStateTarget(rsm),
		TransformationTarget(rsm),
		gctx(gctx),
		rsm(rsm)
	{
	}

	void RenderTarget::Background(const Color & color)
	{
		this->gctx->renderTarget->Clear(D2D1::ColorF(
			(float)color.r / 255.f,
			(float)color.g / 255.f,
			(float)color.b / 255.f,
			1.f
		));
	}

	void RenderTarget::StrokeWeight(float strokeWeight)
	{
		this->rsm->GetActiveState().strokeWeight = strokeWeight;
	}

	void RenderTarget::NoFill()
	{
		this->rsm->GetActiveState().activeFill = nullptr;
	}

	void RenderTarget::NoStroke()
	{
		this->rsm->GetActiveState().activeStroke = nullptr;
	}

	void RenderTarget::Fill(const Color & color)
	{
		RenderState & state = this->rsm->GetActiveState();
		state.solidFill.SetColor(color);
		state.activeFill = state.solidFill.GetBrush();
	}

	void RenderTarget::Stroke(const Color & color)
	{
		RenderState & state = this->rsm->GetActiveState();
		state.solidStroke.SetColor(color);
		state.activeStroke = state.solidStroke.GetBrush();
	}

	void RenderTarget::LinearFill(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->rsm->GetActiveState();
		state.linearFill.SetStart(startX, startY);
		state.linearFill.SetEnd(endX, endY);
		state.linearFill.SetColors(colors);
		state.activeFill = state.linearFill.GetBrush();
	}

	void RenderTarget::LinearStroke(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->rsm->GetActiveState();
		state.linearStroke.SetStart(startX, startY);
		state.linearStroke.SetEnd(endX, endY);
		state.linearStroke.SetColors(colors);
		state.activeStroke = state.linearStroke.GetBrush();
	}

	void RenderTarget::RadialFill(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->rsm->GetActiveState();
		state.radialFill.SetCenter(centerX, centerY);
		state.radialFill.SetRadius(radiusX, radiusY);
		state.radialFill.SetOffset(offsetX, offsetY);
		state.radialFill.SetColors(colors);
		state.activeFill = state.radialFill.GetBrush();
	}

	void RenderTarget::RadialStroke(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->rsm->GetActiveState();
		state.radialStroke.SetCenter(centerX, centerY);
		state.radialStroke.SetRadius(radiusX, radiusY);
		state.radialStroke.SetOffset(offsetX, offsetY);
		state.radialStroke.SetColors(colors);
		state.activeStroke = state.radialStroke.GetBrush();
	}

	void RenderTarget::RectMode(DrawMode mode)
	{
		this->rsm->GetActiveState().rectMode = mode;
	}

	void RenderTarget::Rect(float x1, float y1, float x2, float y2, float cornerRadiusX, float cornerRadiusY)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->renderTarget.Get();
		RenderState & state = this->rsm->GetActiveState();

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
			rt->DrawRoundedRectangle(roundedRect, stroke, state.strokeWeight, state.strokeStyle.GetStrokeStyle());
		}
	}

	void RenderTarget::Quad(float x, float y, float size, float cornerRadiusX, float cornerRadiusY)
	{
		Rect(x, y, size, size, cornerRadiusX, cornerRadiusY);
	}

	void RenderTarget::EllipseMode(DrawMode mode)
	{
		this->rsm->GetActiveState().ellipseMode = mode;
	}

	void RenderTarget::Ellipse(float x, float y, float rx, float ry)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->renderTarget.Get();
		RenderState & state = this->rsm->GetActiveState();

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
			rt->DrawEllipse(ellipse, stroke, state.strokeWeight, state.strokeStyle.GetStrokeStyle());
		}
	}

	void RenderTarget::Circle(float x, float y, float radius)
	{
		this->Ellipse(x, y, radius, radius);
	}

	void RenderTarget::Point(float x, float y)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->renderTarget.Get();
		RenderState & state = this->rsm->GetActiveState();
		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), state.strokeWeight, state.strokeWeight);

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->FillEllipse(ellipse, stroke);
		}
	}

	void RenderTarget::Line(float x1, float y1, float x2, float y2)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->renderTarget.Get();
		RenderState & state = this->rsm->GetActiveState();

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), stroke, state.strokeWeight, state.strokeStyle.GetStrokeStyle());
		}
	}

	void RenderTarget::ImageMode(DrawMode mode)
	{
		this->rsm->GetActiveState().imageMode = mode;
	}

	void RenderTarget::Text(const std::string & string, float x, float y)
	{
		const FVector2 size = this->GetTextSize(string);
		this->Text(string, x, y, size.width, size.height);
	}

	void RenderTarget::Text(const std::string & string, float x, float y, float width)
	{
		const FVector2 size = this->GetTextSize(string);
		this->Text(string, x, y, width, size.height);
	}

	void RenderTarget::Text(const std::string & string, float x, float y, float width, float height)
	{
		RenderState & state = this->rsm->GetActiveState();
		ID2D1HwndRenderTarget * rt = this->gctx->renderTarget.Get();

		TextRenderer & textRenderer = state.textRenderer;
		textRenderer.SetText(string);
		textRenderer.SetMaxWidth(width);
		textRenderer.SetMaxHeight(height);

		IDWriteTextLayout * layout = textRenderer.GetTextLayout();
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

	void RenderTarget::TextSize(float textSize)
	{
		this->rsm->GetActiveState().textRenderer.SetTextSize(textSize);
	}

	void RenderTarget::TextUnderline(bool active)
	{
		this->rsm->GetActiveState().textRenderer.SetUnderline(active);
	}

	void RenderTarget::TextStrikeThrough(bool active)
	{
		this->rsm->GetActiveState().textRenderer.SetStrikeThrough(active);
	}

	void RenderTarget::TextFont(const std::string & font)
	{
		this->rsm->GetActiveState().textRenderer.SetFont(font);
	}

	void RenderTarget::TextFontWeight(FontWeight fontWeight)
	{
		this->rsm->GetActiveState().textRenderer.SetFontWeight(fontWeight);
	}

	void RenderTarget::TextFontStyle(FontStyle fontStyle)
	{
		this->rsm->GetActiveState().textRenderer.SetFontStyle(fontStyle);
	}

	void RenderTarget::TextFontStretch(FontStretch fontStretch)
	{
		this->rsm->GetActiveState().textRenderer.SetFontStretch(fontStretch);
	}

	void RenderTarget::TextWordWrapping(WordWrapping wordWrapping)
	{
		this->rsm->GetActiveState().textRenderer.SetWordWrapping(wordWrapping);
	}

	void RenderTarget::TextAlignment(Core::TextAlignment textAlignment)
	{
		this->rsm->GetActiveState().textRenderer.SetTextAlignment(textAlignment);
	}

	void RenderTarget::TextParagraphAlignment(ParagraphAlignment paragraphAlignment)
	{
		this->rsm->GetActiveState().textRenderer.SetParagraphAlignment(paragraphAlignment);
	}

	FVector2 RenderTarget::GetTextSize()
	{
		return this->rsm->GetActiveState().textRenderer.GetSize();
	}

	FVector2 RenderTarget::GetTextSize(const std::string & content)
	{
		this->rsm->GetActiveState().textRenderer.SetText(content);
		return this->GetTextSize();
	}

	GraphicsContext *& RenderTarget::GetGraphicsContext() const
	{
		return this->gctx;
	}

}