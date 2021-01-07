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
		void push();

		/* pops the current RenderState from the stack. The RenderState below this one will be activated */
		void pop();

		/* resets the matrix to its identity value */
		void resetMatrix();

		/* pushes another matrix. The new matrix advances by the current matrix (they build up on each other) */
		void pushMatrix();
		
		/* pops the current matrix from the stack and activates the matrix below */
		void popMatrix();

		/* translates the to x, y location in screen coordinates (0, 0, width, height) */
		void translate(float x, float y);

		/* rotates by the angle in degrees. */
		void rotate(float degrees);

		/* scales by the given factors */
		void scale(float factorX, float factorY);

		/* clears/erases the background */
		void background(const Color & color);

		/* changes the strokeweight */
		void strokeWeight(float strokeWeight);

		/* deactivates the fill */
		void noFill();

		/* deactivates the outline */
		void noStroke();

		/* activates & changes the fill to a solid color */
		void fill(const Color & color);

		/* activates & changes the outline to a solid color */
		void stroke(const Color & color);

		/* activates & changes the fill to a linear gradient */
		void linearFill(const std::vector<Color> & colors, float startX, float startY, float endX, float endY);
		
		/* activates & changes the outline to a linear gradient */
		void linearStroke(const std::vector<Color> & colors, float startX, float startY, float endX, float endY);

		/* activates & changes the fill to a radial gradient */
		void radialFill(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);
		
		/* activates & changes the outline to a radial gradient */
		void radialStroke(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);

		/* changes the rectmode (the way the parameters of rect(...) are used */
		void rectMode(DrawMode mode);

		/* draws a rectangle based on the rectMode */
		void rect(float x1, float y1, float x2, float y2, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);
		
		/* draws a rectangle with the same width and height */
		void quad(float x, float y, float size, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);

		/* changes the ellipsemode (the way the parameters of ellipse(...) are used) */
		void ellipseMode(DrawMode mode);

		/* draws an ellipe based on the ellipseMode */
		void ellipse(float x, float y, float rx, float ry);
		
		/* draws an ellipsed with the same radii's */
		void circle(float x, float y, float radius);
		
		/* draws a point on screen. It uses the stroke as fill and strokeWeight as radius */
		void point(float x, float y);

		/* draws a line from x1, y1 to x2, y2. It uses the stroke as fill */
		void line(float x1, float y1, float x2, float y2);

		/* changes the image-mode (the way the parameters of image(...) are used) */
		void imageMode(DrawMode mode);
		
		/* begins listening on shape calls (vertex, bezier, ...) */
		void beginShape();

		/* stops listening on shape calls and draws the shape */
		void endShape(ShapeEndType type);

		/* changes the fill-mode of the shape */
		void shapeFillMode(FillMode fillMode);

		/* changes the path segment of the shape */
		void shapePathSegment(PathSegment pathSegment);

		/* puts a vertex (point) into a shape (call this between beginShape() and endShape() */
		void vertex(float x, float y);

		/* puts a quadratic bezier-segment into a shape (call this between beginShape() and endShape() */
		void quadraticBezier(float x1, float y1, float x2, float y2);

		/* puts a bezier-segment (curve) into a shape (call this between beginShape() and endShape() */
		void bezier(float x1, float y1, float x2, float y2, float x3, float y3);

		/* puts an arc-segment into a shape (call this between beginShape() and endShape() */
		void arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection sweepDirection, ArcSize size);

		/* changes the start of the outline */
		void strokeStartCap(CapStyle style);
		
		/* changes the end of the outline */
		void strokeEndCap(CapStyle style);
		
		/* changes the center between the lines of an outline */
		void strokeDashCap(CapStyle style);

		/* changes the join of the outline lines */
		void strokeLineJoin(LineJoin lineJoin);

		/* changes the connection style of the outline */
		void strokeDashStyle(DashStyle style);

		/* changes the pattern of an outline */
		void strokeDashes(const std::vector<float> & dashes);

		/* changes the offset between the outline dashes */
		void strokeDashOffset(float offset);
		
		/* changes the miter limit of the outline dashes */
		void strokeMiterLimit(float limit);

		/* draws text at x, y to the screen */
		void text(const std::string & string, float x, float y);
		
		/* draws text at x, y to the screen with the width as orientation */
		void text(const std::string & string, float x, float y, float width);

		/* draws text at x, y to the screen with the width and height as orientation */
		void text(const std::string & string, float x, float y, float width, float height);

		/* changes the text size */
		void textSize(float textSize);

		/* activates or deactivates underlining text */
		void textUnderline(bool active);

		/* activates or deactivates putting a line through the center of a text */
		void textStrikeThrough(bool active);

		/* changes the font to a font which needs to be in the system font directory */
		void textFont(const std::string & font);

		/* changes the font weight */
		void textFontWeight(FontWeight fontWeight);
		
		/* changes the font style */
		void textFontStyle(FontStyle fontStyle);
		
		/* changes the font stretch */
		void textFontStretch(FontStretch fontStretch);
		
		/* changes the word-wrapping */
		void textWordWrapping(WordWrapping wordWrapping);
		
		/* changes the text alignment (horizontal) */
		void textAlignment(TextAlignment textAlignment);

		/* changes the paragraph alignment (vertical) */
		void textParagraphAlignment(ParagraphAlignment paragraphAlignment);

		/* returns the current text size */
		FVector2 getTextSize();
		
		/* returns the text size of a text object with that content as text */
		FVector2 getTextSize(const std::string & content);

		GraphicsContext *& getGraphicsContext() const;

	private:

		/* instance of a GraphicsContext. Used for drawing routines */
		GraphicsContext *& gctx;

		/* instance of a RenderStateManager. */
		RenderStateManager *& rsm;

	};

}