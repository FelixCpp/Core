#include <Core/Rendering/TextRenderer.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	TextRenderer::TextRenderer(GraphicsContext *& gctx) :
		layout(nullptr),
		range({0u, 0u}),
		fontWeight(FontWeight::Normal),
		fontStyle(FontStyle::Normal),
		fontStretch(FontStretch::Normal),
		wordWrapping(WordWrapping::Wrap),
		textAlignment(TextAlignment::Center),
		paragraphAlignment(ParagraphAlignment::Center),
		text(),
		font("Arial"),
		textSize(12.f),
		maxWidth(INFINITY),
		maxHeight(INFINITY),
		underline(false),
		strikeThrough(false),
		gctx(gctx)
	{
	}

	void TextRenderer::setText(const std::string & text)
	{
		if (this->text != text)
		{
			this->text = text;
			this->updateText();
		}
	}

	const std::string & TextRenderer::getText() const
	{
		return this->text;
	}

	void TextRenderer::setFont(const std::string & font)
	{
		if (this->font != font)
		{
			this->font = font;
			this->updateFont();
		}
	}

	const std::string & TextRenderer::getFont() const
	{
		return this->font;
	}

	void TextRenderer::setFontWeight(FontWeight fontWeight)
	{
		if (this->fontWeight != fontWeight)
		{
			this->fontWeight = fontWeight;
			this->updateFontWeight();
		}
	}

	FontWeight TextRenderer::getFontWeight() const
	{
		return this->fontWeight;
	}

	void TextRenderer::setFontStyle(FontStyle fontStyle)
	{
		if (this->fontStyle != fontStyle)
		{
			this->fontStyle = fontStyle;
			this->updateFontStyle();
		}
	}

	FontStyle TextRenderer::getFontStyle() const
	{
		return this->fontStyle;
	}

	void TextRenderer::setFontStretch(FontStretch fontStretch)
	{
		if (this->fontStretch != fontStretch)
		{
			this->fontStretch = fontStretch;
			this->updateFontStretch();
		}
	}

	FontStretch TextRenderer::getFontStretch() const
	{
		return this->fontStretch;
	}

	void TextRenderer::setWordWrapping(WordWrapping wordWrapping)
	{
		if (this->wordWrapping != wordWrapping)
		{
			this->wordWrapping = wordWrapping;
			this->updateWordWrapping();
		}
	}

	WordWrapping TextRenderer::getWordWrapping() const
	{
		return this->wordWrapping;
	}

	void TextRenderer::setTextAlignment(TextAlignment textAlignment)
	{
		if (this->textAlignment != textAlignment)
		{
			this->textAlignment = textAlignment;
			this->updateTextAlignment();
		}
	}

	TextAlignment TextRenderer::getTextAlignment() const
	{
		return this->textAlignment;
	}

	void TextRenderer::setParagraphAlignment(ParagraphAlignment paragraphAlignment)
	{
		if (this->paragraphAlignment != paragraphAlignment)
		{
			this->paragraphAlignment = paragraphAlignment;
			this->updateParagrapAlignment();
		}
	}

	ParagraphAlignment TextRenderer::getParagraphAlignment() const
	{
		return this->paragraphAlignment;
	}

	void TextRenderer::setTextSize(float textSize)
	{
		if (this->textSize != textSize)
		{
			this->textSize = textSize;
			this->updateTextSize();
		}
	}

	float TextRenderer::getTextSize() const
	{
		return this->textSize;
	}

	void TextRenderer::setUnderline(bool active)
	{
		if (this->underline != active)
		{
			this->underline = active;
			this->updateUnderline();
		}
	}

	bool TextRenderer::HasUnderline() const
	{
		return this->underline;
	}

	void TextRenderer::setStrikeThrough(bool active)
	{
		if (this->strikeThrough != active)
		{
			this->strikeThrough = active;
			this->updateStrikeThrough();
		}
	}

	bool TextRenderer::HasStrikeThrough() const
	{
		return this->strikeThrough;
	}

	void TextRenderer::setMaxWidth(float width)
	{
		if (this->maxWidth != width)
		{
			this->maxWidth = width;
			this->updateMaxWidth();
		}
	}

	float TextRenderer::getMaxWidth() const
	{
		return this->maxWidth;
	}

	void TextRenderer::setMaxHeight(float height)
	{
		if (this->maxHeight != height)
		{
			this->maxHeight = height;
			this->updateMaxHeight();
		}
	}

	float TextRenderer::getMaxHeight() const
	{
		return this->maxHeight;
	}

	FVector2 TextRenderer::getSize() const
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
		{
			DWRITE_TEXT_METRICS metrics = {};
			layout->GetMetrics(&metrics);
			return FVector2(metrics.width, metrics.height);
		}

		return FVector2(0.f, 0.f);
	}

	IDWriteTextLayout * TextRenderer::getTextLayout() const
	{
		return this->layout.Get();
	}

	void TextRenderer::updateText()
	{
		IDWriteFactory * writeFactory = this->gctx->writeFactory.Get();
		const std::wstring wFont(this->font.begin(), this->font.end());
		Microsoft::WRL::ComPtr<IDWriteTextFormat> format = nullptr;
		HRESULT hr = writeFactory->CreateTextFormat(
			wFont.c_str(),
			nullptr,
			static_cast<DWRITE_FONT_WEIGHT>(this->fontWeight),
			static_cast<DWRITE_FONT_STYLE>(this->fontStyle),
			static_cast<DWRITE_FONT_STRETCH>(this->fontStretch),
			this->textSize,
			L"en-us",
			&format
		);
		if (FAILED(hr))
		{
			CORE_ERROR("CreateTextFormat");
			return;
		}

		const std::wstring string(this->text.begin(), this->text.end());
		hr = writeFactory->CreateTextLayout(
			string.c_str(),
			string.length(),
			format.Get(),
			this->maxWidth,
			this->maxHeight,
			&this->layout
		);
		if (FAILED(hr))
		{
			CORE_ERROR("CreateTextLayout");
			return;
		}
		
		this->range.startPosition = 0;
		this->range.length = string.length();

		this->layout->SetStrikethrough(this->strikeThrough, this->range);
		this->layout->SetUnderline(this->underline, this->range);
		this->layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(this->wordWrapping));
		this->layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(this->textAlignment));
		this->layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(this->paragraphAlignment));
	}

	void TextRenderer::updateFont()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
		{
			const std::wstring wFont(this->font.begin(), this->font.end());
			layout->SetFontFamilyName(wFont.c_str(), this->range);
		}
	}

	void TextRenderer::updateFontWeight()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontWeight(static_cast<DWRITE_FONT_WEIGHT>(this->fontWeight), this->range);
	}

	void TextRenderer::updateFontStyle()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontStyle(static_cast<DWRITE_FONT_STYLE>(this->fontStyle), this->range);
	}

	void TextRenderer::updateFontStretch()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontStretch(static_cast<DWRITE_FONT_STRETCH>(this->fontStretch), this->range);
	}

	void TextRenderer::updateWordWrapping()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(this->wordWrapping));
	}

	void TextRenderer::updateTextAlignment()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(this->textAlignment));
	}

	void TextRenderer::updateParagrapAlignment()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(this->paragraphAlignment));
	}

	void TextRenderer::updateTextSize()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontSize(this->textSize, this->range);
	}

	void TextRenderer::updateMaxWidth()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetMaxWidth(this->maxWidth);
	}

	void TextRenderer::updateMaxHeight()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetMaxHeight(this->maxHeight);
	}

	void TextRenderer::updateUnderline()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetUnderline(this->underline, this->range);
	}

	void TextRenderer::updateStrikeThrough()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetStrikethrough(this->strikeThrough, this->range);
	}

}