#pragma once

#include <Core/Rendering/Helpers.hpp>
#include <Core/Rendering/DrawMode.hpp>
#include <Core/Rendering/ImageTarget.hpp>
#include <Core/Rendering/ShapeAttributes.hpp>
#include <Core/Rendering/TextAttributes.hpp>
#include <Core/Rendering/StrokeStyleAttributes.hpp>
#include <Core/Maths/Vector2.hpp>
#include <Core/Rendering/Color.hpp>

#include <vector>

namespace Core
{

	class GraphicsContext;

	class RenderTarget : public ImageTarget {
	public:

		/* RenderTarget constructor */
		explicit RenderTarget(GraphicsContext *& gctx, RenderStateManager *& rsm);
		
		/* virtual destructor */
		virtual ~RenderTarget() = default;

		/* pushes a new RenderState onto the stack. fill/stroke etc. will be resetted */
		void Push();

		/* pops the current RenderState from the stack. The RenderState below this one will be activated */
		void Pop();

		/* resets the matrix to its identity value */
		void ResetMatrix();

		/* pushes another matrix. The new matrix advances by the current matrix (they build up on each other) */
		void PushMatrix();
		
		/* pops the current matrix from the stack and activates the matrix below */
		void PopMatrix();

		/* translates the to x, y location in screen coordinates (0, 0, width, height) */
		void Translate(float x, float y);

		/* rotates by the angle in degrees. */
		void Rotate(float degrees);

		/* scales by the given factors */
		void Scale(float factorX, float factorY);

		/* clears/erases the background */
		void Background(const Color & color);

		/* changes the strokeweight */
		void StrokeWeight(float strokeWeight);

		/* deactivates the fill */
		void NoFill();

		/* deactivates the outline */
		void NoStroke();

		/* activates & changes the fill to a solid color */
		void Fill(const Color & color);

		/* activates & changes the outline to a solid color */
		void Stroke(const Color & color);

		/* activates & changes the fill to a linear gradient */
		void LinearFill(const std::vector<Color> & colors, float startX, float startY, float endX, float endY);
		
		/* activates & changes the outline to a linear gradient */
		void LinearStroke(const std::vector<Color> & colors, float startX, float startY, float endX, float endY);

		/* activates & changes the fill to a radial gradient */
		void RadialFill(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);
		
		/* activates & changes the outline to a radial gradient */
		void RadialStroke(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);

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

		/* changes the image-mode (the way the parameters of image(...) are used) */
		void ImageMode(DrawMode mode);
		
		/* begins listening on shape calls (vertex, bezier, ...) */
		void BeginShape(ShapeBegin begin = ShapeBegin::Filled);

		/* stops listening on shape calls and draws the shape */
		void EndShape(ShapeEnd end = ShapeEnd::Open);

		/* changes the fill-mode of the shape */
		void ShapeFillMode(FillMode fillMode);

		/* changes the path segment of the shape */
		void ShapePathSegment(PathSegment pathSegment);

		/* puts a vertex (point) into a shape (call this between beginShape() and endShape() */
		void Vertex(float x, float y);

		/* puts a quadratic bezier-segment into a shape (call this between beginShape() and endShape() */
		void QuadraticBezier(float x1, float y1, float x2, float y2);

		/* puts a bezier-segment (curve) into a shape (call this between beginShape() and endShape() */
		void Bezier(float x1, float y1, float x2, float y2, float x3, float y3);

		/* puts an arc-segment into a shape (call this between beginShape() and endShape() */
		void Arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection direction, ArcSize size);

		/* changes the start of the outline */
		void StrokeStartCap(CapStyle style);
		
		/* changes the end of the outline */
		void StrokeEndCap(CapStyle style);
		
		/* changes the center between the lines of an outline */
		void StrokeDashCap(CapStyle style);

		/* changes the join of the outline lines */
		void StrokeLineJoin(LineJoin lineJoin);

		/* changes the connection style of the outline */
		void StrokeDashStyle(DashStyle style);

		/* changes the pattern of an outline */
		void StrokeDashes(const std::vector<float> & dashes);

		/* changes the offset between the outline dashes */
		void StrokeDashOffset(float offset);
		
		/* changes the miter limit of the outline dashes */
		void StrokeMiterLimit(float limit);

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