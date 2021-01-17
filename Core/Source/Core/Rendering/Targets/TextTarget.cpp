#include <Core/Rendering/Targets/TextTarget.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderState.hpp>

namespace Core
{

	TextTarget::TextTarget(GraphicsContext *& gctx, RenderStateManager *& rsm) :
		gctx(gctx),
		rsm(rsm)
	{ }

	void TextTarget::Text(const std::string & string, float x, float y)
	{
		const FVector2 size = this->GetTextSize(string);
		this->Text(string, x, y, size.width, size.height);
	}

	void TextTarget::Text(const std::string & string, float x, float y, float width)
	{
		const FVector2 size = this->GetTextSize(string);
		this->Text(string, x, y, width, size.height);
	}

	void TextTarget::Text(const std::string & string, float x, float y, float width, float height)
	{
		RenderState & state = this->GetState();
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

	void TextTarget::TextSize(float textSize)
	{
		this->GetState().textRenderer.SetTextSize(textSize);
	}

	void TextTarget::TextUnderline(bool active)
	{
		this->GetState().textRenderer.SetUnderline(active);
	}

	void TextTarget::TextStrikeThrough(bool active)
	{
		this->GetState().textRenderer.SetStrikeThrough(active);
	}

	void TextTarget::TextFont(const std::string & font)
	{
		this->GetState().textRenderer.SetFont(font);
	}

	void TextTarget::TextFontWeight(FontWeight fontWeight)
	{
		this->GetState().textRenderer.SetFontWeight(fontWeight);
	}

	void TextTarget::TextFontStyle(FontStyle fontStyle)
	{
		this->GetState().textRenderer.SetFontStyle(fontStyle);
	}

	void TextTarget::TextFontStretch(FontStretch fontStretch)
	{
		this->GetState().textRenderer.SetFontStretch(fontStretch);
	}

	void TextTarget::TextWordWrapping(WordWrapping wordWrapping)
	{
		this->GetState().textRenderer.SetWordWrapping(wordWrapping);
	}

	void TextTarget::TextAlignment(Core::TextAlignment textAlignment)
	{
		this->GetState().textRenderer.SetTextAlignment(textAlignment);
	}

	void TextTarget::TextParagraphAlignment(ParagraphAlignment paragraphAlignment)
	{
		this->GetState().textRenderer.SetParagraphAlignment(paragraphAlignment);
	}

	FVector2 TextTarget::GetTextSize()
	{
		return this->GetState().textRenderer.GetSize();
	}

	FVector2 TextTarget::GetTextSize(const std::string & content)
	{
		this->GetState().textRenderer.SetText(content);
		return this->GetTextSize();
	}

	RenderState & TextTarget::GetState()
	{
		return this->rsm->GetActiveState();
	}


}