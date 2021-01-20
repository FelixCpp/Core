#include <Core/Rendering/TextRenderer.hpp> // Core::TextRenderer
#include <Core/Rendering/FactoryManager.hpp> // Core::FactoryManager

#include <Core/System/Logger.hpp> // CORE_ERROR

namespace Core
{

	TextRenderer::TextRenderer() :
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
		strikeThrough(false)
	{
	}

	void TextRenderer::SetText(const std::string & text)
	{
		if (this->text != text)
		{
			this->text = text;
			this->UpdateText();
		}
	}

	const std::string & TextRenderer::GetText() const
	{
		return this->text;
	}

	void TextRenderer::SetFont(const std::string & font)
	{
		if (this->font != font)
		{
			this->font = font;
			this->UpdateFont();
		}
	}

	const std::string & TextRenderer::GetFont() const
	{
		return this->font;
	}

	void TextRenderer::SetFontWeight(FontWeight fontWeight)
	{
		if (this->fontWeight != fontWeight)
		{
			this->fontWeight = fontWeight;
			this->UpdateFontWeight();
		}
	}

	FontWeight TextRenderer::GetFontWeight() const
	{
		return this->fontWeight;
	}

	void TextRenderer::SetFontStyle(FontStyle fontStyle)
	{
		if (this->fontStyle != fontStyle)
		{
			this->fontStyle = fontStyle;
			this->UpdateFontStyle();
		}
	}

	FontStyle TextRenderer::GetFontStyle() const
	{
		return this->fontStyle;
	}

	void TextRenderer::SetFontStretch(FontStretch fontStretch)
	{
		if (this->fontStretch != fontStretch)
		{
			this->fontStretch = fontStretch;
			this->UpdateFontStretch();
		}
	}

	FontStretch TextRenderer::GetFontStretch() const
	{
		return this->fontStretch;
	}

	void TextRenderer::SetWordWrapping(WordWrapping wordWrapping)
	{
		if (this->wordWrapping != wordWrapping)
		{
			this->wordWrapping = wordWrapping;
			this->UpdateWordWrapping();
		}
	}

	WordWrapping TextRenderer::GetWordWrapping() const
	{
		return this->wordWrapping;
	}

	void TextRenderer::SetTextAlignment(TextAlignment textAlignment)
	{
		if (this->textAlignment != textAlignment)
		{
			this->textAlignment = textAlignment;
			this->UpdateTextAlignment();
		}
	}

	TextAlignment TextRenderer::GetTextAlignment() const
	{
		return this->textAlignment;
	}

	void TextRenderer::SetParagraphAlignment(ParagraphAlignment paragraphAlignment)
	{
		if (this->paragraphAlignment != paragraphAlignment)
		{
			this->paragraphAlignment = paragraphAlignment;
			this->UpdateParagrapAlignment();
		}
	}

	ParagraphAlignment TextRenderer::GetParagraphAlignment() const
	{
		return this->paragraphAlignment;
	}

	void TextRenderer::SetTextSize(float textSize)
	{
		if (this->textSize != textSize)
		{
			this->textSize = textSize;
			this->UpdateTextSize();
		}
	}

	float TextRenderer::GetTextSize() const
	{
		return this->textSize;
	}

	void TextRenderer::SetUnderline(bool active)
	{
		if (this->underline != active)
		{
			this->underline = active;
			this->UpdateUnderline();
		}
	}

	bool TextRenderer::HasUnderline() const
	{
		return this->underline;
	}

	void TextRenderer::SetStrikeThrough(bool active)
	{
		if (this->strikeThrough != active)
		{
			this->strikeThrough = active;
			this->UpdateStrikeThrough();
		}
	}

	bool TextRenderer::HasStrikeThrough() const
	{
		return this->strikeThrough;
	}

	void TextRenderer::SetMaxWidth(float width)
	{
		if (this->maxWidth != width)
		{
			this->maxWidth = width;
			this->UpdateMaxWidth();
		}
	}

	float TextRenderer::GetMaxWidth() const
	{
		return this->maxWidth;
	}

	void TextRenderer::SetMaxHeight(float height)
	{
		if (this->maxHeight != height)
		{
			this->maxHeight = height;
			this->UpdateMaxHeight();
		}
	}

	float TextRenderer::GetMaxHeight() const
	{
		return this->maxHeight;
	}

	FVector2 TextRenderer::GetSize() const
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
		{
			DWRITE_TEXT_METRICS metrics = {};
			layout->GetMetrics(&metrics);
			return FVector2(metrics.width, metrics.height);
		}

		return FVector2(0.f, 0.f);
	}

	IDWriteTextLayout * TextRenderer::GetTextLayout() const
	{
		return this->layout.Get();
	}

	void TextRenderer::UpdateText()
	{
		IDWriteFactory * writeFactory = FactoryManager::dwriteFactory.Get();
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
			(UINT32)string.length(),
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
		this->range.length = (UINT32)string.length();

		this->layout->SetStrikethrough(this->strikeThrough, this->range);
		this->layout->SetUnderline(this->underline, this->range);
		this->layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(this->wordWrapping));
		this->layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(this->textAlignment));
		this->layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(this->paragraphAlignment));
	}

	void TextRenderer::UpdateFont()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
		{
			const std::wstring wFont(this->font.begin(), this->font.end());
			layout->SetFontFamilyName(wFont.c_str(), this->range);
		}
	}

	void TextRenderer::UpdateFontWeight()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontWeight(static_cast<DWRITE_FONT_WEIGHT>(this->fontWeight), this->range);
	}

	void TextRenderer::UpdateFontStyle()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontStyle(static_cast<DWRITE_FONT_STYLE>(this->fontStyle), this->range);
	}

	void TextRenderer::UpdateFontStretch()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontStretch(static_cast<DWRITE_FONT_STRETCH>(this->fontStretch), this->range);
	}

	void TextRenderer::UpdateWordWrapping()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(this->wordWrapping));
	}

	void TextRenderer::UpdateTextAlignment()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(this->textAlignment));
	}

	void TextRenderer::UpdateParagrapAlignment()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(this->paragraphAlignment));
	}

	void TextRenderer::UpdateTextSize()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetFontSize(this->textSize, this->range);
	}

	void TextRenderer::UpdateMaxWidth()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetMaxWidth(this->maxWidth);
	}

	void TextRenderer::UpdateMaxHeight()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetMaxHeight(this->maxHeight);
	}

	void TextRenderer::UpdateUnderline()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetUnderline(this->underline, this->range);
	}

	void TextRenderer::UpdateStrikeThrough()
	{
		if (IDWriteTextLayout * layout = this->layout.Get())
			layout->SetStrikethrough(this->strikeThrough, this->range);
	}

}