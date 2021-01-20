#pragma once

#include <string>

#include <Core/Maths/Vector2.hpp>

#include <Core/Rendering/TextAttributes.hpp>

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class RenderState;
	class RenderStateManager;
	class Renderer;

	/// <summary>
	/// A Target class to handle
	/// everything based around
	/// text
	/// </summary>
	class TextRenderTarget {
	public:

		/// <summary>
		/// constructor which passes the GraphicsContext & RenderStateManager
		/// to its member variables.
		/// </summary>
		/// <param name="renderer">the Renderer which is needed to draw the text</param>
		/// <param name="rsm">the RenderStateManager which is needed to get access to the current RenderState holding a text object</param>
		explicit TextRenderTarget(Renderer *& renderer, RenderStateManager *& rsm);

		/// <summary>
		/// A virtual destructor
		/// </summary>
		virtual ~TextRenderTarget() = default;

		/// <summary>
		/// Draws the text to the screen
		/// at the given location
		/// </summary>
		/// <param name="string">string to draw</param>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		void Text(const std::string & string, float x, float y);

		/// <summary>
		/// Draws the text to the screen
		/// at the given location
		/// using the width as orientation
		/// </summary>
		/// <param name="string">string to draw</param>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		/// <param name="width">width used by TextAlignment and ParagraphAlignment</param>
		void Text(const std::string & string, float x, float y, float width);

		/// <summary>
		/// Draws the text to the screen
		/// at the given location
		/// using the width as orientation
		/// </summary>
		/// <param name="string">string to draw</param>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		/// <param name="width">width used by TextAlignment and ParagraphAlignment</param>
		/// <param name="height">height used by TextAlignment and ParagraphAlignment</param>
		void Text(const std::string & string, float x, float y, float width, float height);

		/// <summary>
		/// Changes the font/text-size
		/// The FontSize will be equal to the height which
		/// is used to display the text
		/// </summary>
		/// <param name="textSize">font/text-size</param>
		void TextSize(float textSize);

		/// <summary>
		/// Activates or deactivates an underline below
		/// the text which gets drawn to the screen
		/// </summary>
		/// <param name="active">a boolean to indicate wether there should be an underline or not</param>
		void TextUnderline(bool active);

		/// <summary>
		/// Activates or deactivates a strike through
		/// the text which gets drawn to the screen
		/// </summary>
		/// <param name="active">a boolean to indicate wether there should be a line through the text or not</param>
		void TextStrikeThrough(bool active);

		/// <summary>
		/// Changes the font of the text
		/// </summary>
		/// <param name="font">the name of a font file in the Windows system font folder</param>
		void TextFont(const std::string & font);

		/// <summary>
		/// Changes the fontweight
		/// </summary>
		/// <param name="fontWeight">the new style</param>
		void TextFontWeight(FontWeight fontWeight);

		/// <summary>
		/// Changes the fontstyle
		/// </summary>
		/// <param name="fontStyle">the new style</param>
		void TextFontStyle(FontStyle fontStyle);

		/// <summary>
		/// Changes the fontstretch
		/// </summary>
		/// <param name="fontStretch">the new style</param>
		void TextFontStretch(FontStretch fontStretch);

		/// <summary>
		/// Changes the wordwrapping
		/// </summary>
		/// <param name="wordWrapping">the new style</param>
		void TextWordWrapping(WordWrapping wordWrapping);

		/// <summary>
		/// Changes the textalignment (horizontal)
		/// </summary>
		/// <param name="textAlignment">the new alignment</param>
		void TextAlignment(TextAlignment textAlignment);

		/// <summary>
		/// Changes the paragraph alignment (vertical)
		/// </summary>
		/// <param name="paragraphAlignment">the new alignment</param>
		void TextParagraphAlignment(ParagraphAlignment paragraphAlignment);

		/// <summary>
		/// Returns the boundary in pixels of the last text drawn to the screen
		/// </summary>
		FVector2 GetTextSize();

		/// <summary>
		/// Returns the boundary in pixels of the text passed into the method
		/// </summary>
		/// <param name="content">the text calculate the boundary with</param>
		FVector2 GetTextSize(const std::string & content);

	private:

		/// <summary>
		/// returns the current shape of the
		/// active RenderState
		/// </summary>
		RenderState & GetState();

	private:

		/// <summary>
		/// An instance of the GraphicsContext
		/// to draw the shape
		/// </summary>
		Renderer *& renderer;

		/// <summary>
		/// An instance of the RenderStateManager
		/// to get access to the brushes / drawing
		/// properties
		/// </summary>
		RenderStateManager *& rsm;

	};

	/// <summary>
	/// Example:
	/// <code>
	/// 
	/// ...
	/// TextAlignment(Core::TextAlign::Center);
	/// ParagraphAlignment(Core::ParagraphAlignment::Center);
	/// TextFont("Arial");
	/// TextSize(42.f);
	/// Text("Hello World", 0.f, 0.f, width, height);
	/// ...
	/// 
	/// </code>
	/// </summary>

}