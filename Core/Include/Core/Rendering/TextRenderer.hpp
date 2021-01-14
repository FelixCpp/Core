#pragma once

#include <Core/Maths/Vector2.hpp>
#include <Core/Rendering/TextAttributes.hpp>

#include <memory>
#include <string>

#include <dwrite.h>
#include <wrl/client.h>

namespace Core
{

	class GraphicsContext;

	class TextRenderer {
	public:

		explicit TextRenderer(GraphicsContext *& gctx);

		void SetText(const std::string & text);
		const std::string & GetText() const;

		void SetFont(const std::string & font);
		const std::string & GetFont() const;

		void SetFontWeight(FontWeight fontWeight);
		FontWeight GetFontWeight() const;

		void SetFontStyle(FontStyle fontStyle);
		FontStyle GetFontStyle() const;

		void SetFontStretch(FontStretch fontStretch);
		FontStretch GetFontStretch() const;

		void SetWordWrapping(WordWrapping wordWrapping);
		WordWrapping GetWordWrapping() const;

		void SetTextAlignment(TextAlignment textAlignment);
		TextAlignment GetTextAlignment() const;
		
		void SetParagraphAlignment(ParagraphAlignment paragraphAlignment);
		ParagraphAlignment GetParagraphAlignment() const;

		void SetTextSize(float textSize);
		float GetTextSize() const;

		void SetUnderline(bool active);
		bool HasUnderline() const;

		void SetStrikeThrough(bool active);
		bool HasStrikeThrough() const;

		void SetMaxWidth(float width);
		float GetMaxWidth() const;

		void SetMaxHeight(float height);
		float GetMaxHeight() const;

		FVector2 GetSize() const;

		IDWriteTextLayout * GetTextLayout() const;

	private:

		void UpdateText();
		void UpdateFont();
		void UpdateFontWeight();
		void UpdateFontStyle();
		void UpdateFontStretch();
		void UpdateWordWrapping();
		void UpdateTextAlignment();
		void UpdateParagrapAlignment();
		void UpdateTextSize();
		void UpdateMaxWidth();
		void UpdateMaxHeight();
		void UpdateUnderline();
		void UpdateStrikeThrough();

	private:

		Microsoft::WRL::ComPtr<IDWriteTextLayout> layout;
		DWRITE_TEXT_RANGE range;

		FontWeight fontWeight;
		FontStyle fontStyle;
		FontStretch fontStretch;
		WordWrapping wordWrapping;
		TextAlignment textAlignment;
		ParagraphAlignment paragraphAlignment;

		std::string text;
		std::string font;
		float textSize;
		float maxWidth;
		float maxHeight;

		bool underline;
		bool strikeThrough;

		GraphicsContext *& gctx;
		
	};

}