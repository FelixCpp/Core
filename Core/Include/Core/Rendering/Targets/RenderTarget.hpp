#pragma once

#include <Core/Rendering/Helpers.hpp>
#include <Core/Rendering/DrawMode.hpp>
#include <Core/Rendering/TextAttributes.hpp>
#include <Core/Maths/Vector2.hpp>
#include <Core/Rendering/Color.hpp>

#include <Core/Rendering/Targets/ImageTarget.hpp>
#include <Core/Rendering/Targets/ShapeTarget.hpp>
#include <Core/Rendering/Targets/StrokeStyleTarget.hpp>
#include <Core/Rendering/Targets/RenderStateTarget.hpp>
#include <Core/Rendering/Targets/TransformationTarget.hpp>
#include <Core/Rendering/Targets/BrushTarget.hpp>

#include <vector>

namespace Core
{

	class GraphicsContext;

	class RenderTarget :
		public ImageTarget,
		public ShapeTarget,
		public StrokeStyleTarget,
		public RenderStateTarget,
		public TransformationTarget,
		public BrushTarget
	{
	public:

		/* RenderTarget constructor */
		explicit RenderTarget(GraphicsContext *& gctx, RenderStateManager *& rsm);
		
		/* virtual destructor */
		virtual ~RenderTarget() = default;

		/* clears/erases the background */
		void Background(const Color & color);

		/* changes the rectmode (the way the parameters of rect(...) are used */
		void RectMode(DrawMode mode);

		/* draws a rectangle based on the rectMode */
		void Rect(float x1, float y1, float x2, float y2, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);
		
		/* draws a rectangle with the same width and height */
		void Quad(float x, float y, float size, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);

		/* changes the ellipsemode (the way the parameters of ellipse(...) are used) */
		void EllipseMode(DrawMode mode);

		/* draws an ellipe based on the ellipseMode */
		void Ellipse(float x, float y, float rx, float ry);
		
		/* draws an ellipsed with the same radii's */
		void Circle(float x, float y, float radius);
		
		/* draws a point on screen. It uses the stroke as fill and strokeWeight as radius */
		void Point(float x, float y);

		/* draws a line from x1, y1 to x2, y2. It uses the stroke as fill */
		void Line(float x1, float y1, float x2, float y2);

		/* draws text at x, y to the screen */
		void Text(const std::string & string, float x, float y);
		
		/* draws text at x, y to the screen with the width as orientation */
		void Text(const std::string & string, float x, float y, float width);

		/* draws text at x, y to the screen with the width and height as orientation */
		void Text(const std::string & string, float x, float y, float width, float height);

		/* changes the text size */
		void TextSize(float textSize);

		/* activates or deactivates underlining text */
		void TextUnderline(bool active);

		/* activates or deactivates putting a line through the center of a text */
		void TextStrikeThrough(bool active);

		/* changes the font to a font which needs to be in the system font directory */
		void TextFont(const std::string & font);

		/* changes the font weight */
		void TextFontWeight(FontWeight fontWeight);
		
		/* changes the font style */
		void TextFontStyle(FontStyle fontStyle);
		
		/* changes the font stretch */
		void TextFontStretch(FontStretch fontStretch);
		
		/* changes the word-wrapping */
		void TextWordWrapping(WordWrapping wordWrapping);
		
		/* changes the text alignment (horizontal) */
		void TextAlignment(TextAlignment textAlignment);

		/* changes the paragraph alignment (vertical) */
		void TextParagraphAlignment(ParagraphAlignment paragraphAlignment);

		/* returns the current text size */
		FVector2 GetTextSize();
		
		/* returns the text size of a text object with that content as text */
		FVector2 GetTextSize(const std::string & content);

		GraphicsContext *& GetGraphicsContext() const;

	private:

		/* instance of a GraphicsContext. Used for drawing routines */
		GraphicsContext *& gctx;

		/* instance of a RenderStateManager. */
		RenderStateManager *& rsm;

	};

}