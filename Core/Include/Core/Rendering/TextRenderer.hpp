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

		void setText(const std::string & text);
		const std::string & getText() const;

		void setFont(const std::string & font);
		const std::string & getFont() const;

		void setFontWeight(FontWeight fontWeight);
		FontWeight getFontWeight() const;

		void setFontStyle(FontStyle fontStyle);
		FontStyle getFontStyle() const;

		void setFontStretch(FontStretch fontStretch);
		FontStretch getFontStretch() const;

		void setWordWrapping(WordWrapping wordWrapping);
		WordWrapping getWordWrapping() const;

		void setTextAlignment(TextAlignment textAlignment);
		TextAlignment getTextAlignment() const;
		
		void setParagraphAlignment(ParagraphAlignment paragraphAlignment);
		ParagraphAlignment getParagraphAlignment() const;

		void setTextSize(float textSize);
		float getTextSize() const;

		void setUnderline(bool active);
		bool HasUnderline() const;

		void setStrikeThrough(bool active);
		bool HasStrikeThrough() const;

		void setMaxWidth(float width);
		float getMaxWidth() const;

		void setMaxHeight(float height);
		float getMaxHeight() const;

		FVector2 getSize() const;

		IDWriteTextLayout * getTextLayout() const;

	private:

		void updateText();
		void updateFont();
		void updateFontWeight();
		void updateFontStyle();
		void updateFontStretch();
		void updateWordWrapping();
		void updateTextAlignment();
		void updateParagrapAlignment();
		void updateTextSize();
		void updateMaxWidth();
		void updateMaxHeight();
		void updateUnderline();
		void updateStrikeThrough();

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