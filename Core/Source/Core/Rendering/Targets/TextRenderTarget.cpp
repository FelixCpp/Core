#include <Core/Rendering/Targets/TextRenderTarget.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/RenderState.hpp>

namespace Core
{

	TextRenderTarget::TextRenderTarget(LateRef<Renderer> renderer, LateRef<RenderStateManager> rsm) :
		renderer(renderer),
		rsm(rsm)
	{ }

	void TextRenderTarget::Text(const std::string & string, float x, float y)
	{
		const FVector2 size = this->GetTextSize(string);
		this->Text(string, x, y, size.width, size.height);
	}

	void TextRenderTarget::Text(const std::string & string, float x, float y, float width)
	{
		const FVector2 size = this->GetTextSize(string);
		this->Text(string, x, y, width, size.height);
	}

	void TextRenderTarget::Text(const std::string & string, float x, float y, float width, float height)
	{
		RenderState & state = this->GetState();
		ID2D1RenderTarget * rt = this->renderer->GetRenderTarget();

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

	void TextRenderTarget::TextSize(float textSize)
	{
		this->GetState().textRenderer.SetTextSize(textSize);
	}

	void TextRenderTarget::TextUnderline(bool active)
	{
		this->GetState().textRenderer.SetUnderline(active);
	}

	void TextRenderTarget::TextStrikeThrough(bool active)
	{
		this->GetState().textRenderer.SetStrikeThrough(active);
	}

	void TextRenderTarget::TextFont(const std::string & font)
	{
		this->GetState().textRenderer.SetFont(font);
	}

	void TextRenderTarget::TextFontWeight(FontWeight fontWeight)
	{
		this->GetState().textRenderer.SetFontWeight(fontWeight);
	}

	void TextRenderTarget::TextFontStyle(FontStyle fontStyle)
	{
		this->GetState().textRenderer.SetFontStyle(fontStyle);
	}

	void TextRenderTarget::TextFontStretch(FontStretch fontStretch)
	{
		this->GetState().textRenderer.SetFontStretch(fontStretch);
	}

	void TextRenderTarget::TextWordWrapping(WordWrapping wordWrapping)
	{
		this->GetState().textRenderer.SetWordWrapping(wordWrapping);
	}

	void TextRenderTarget::TextAlignment(Core::TextAlignment textAlignment)
	{
		this->GetState().textRenderer.SetTextAlignment(textAlignment);
	}

	void TextRenderTarget::TextParagraphAlignment(ParagraphAlignment paragraphAlignment)
	{
		this->GetState().textRenderer.SetParagraphAlignment(paragraphAlignment);
	}

	FVector2 TextRenderTarget::GetTextSize()
	{
		return this->GetState().textRenderer.GetSize();
	}

	FVector2 TextRenderTarget::GetTextSize(const std::string & content)
	{
		this->GetState().textRenderer.SetText(content);
		return this->GetTextSize();
	}

	RenderState & TextRenderTarget::GetState()
	{
		return this->rsm->GetActiveState();
	}


}