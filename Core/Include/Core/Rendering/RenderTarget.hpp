#pragma once

#include <Core/Rendering/Helpers.hpp>
#include <Core/Rendering/DrawMode.hpp>
#include <Core/Rendering/Image.hpp>
#include <Core/Rendering/ShapeAttributes.hpp>
#include <Core/Rendering/TextAttributes.hpp>
#include <Core/Rendering/StrokeStyleAttributes.hpp>
#include <Core/Maths/Vector2.hpp>

#include <vector>

namespace Core
{

	class GraphicsContext;

	class RenderTarget {
	public:

		explicit RenderTarget(GraphicsContext *& gctx);
		virtual ~RenderTarget() = default;

		Image loadImage(const std::string & filepath);
		Image createImage(u32_t width, u32_t height, color_t color = Color::White, i32_t channels = 4, i32_t opacity = 255, ImageInterpolationMode mode = ImageInterpolationMode::NearestNeighbor);

		void push();
		void pop();

		void resetMatrix();
		void pushMatrix();
		void popMatrix();
		void translate(float x, float y);
		void rotate(float degrees);
		void scale(float factorX, float factorY);

		void background(const color_t & color_t);
		void strokeWeight(float strokeWeight);

		void noFill();
		void noStroke();

		void fill(const color_t & color_t);
		void stroke(const color_t & color_t);

		void linearFill(const std::vector<color_t> & ColorTypes, float startX, float startY, float endX, float endY);
		void linearStroke(const std::vector<color_t> & ColorTypes, float startX, float startY, float endX, float endY);

		void radialFill(const std::vector<color_t> & ColorTypes, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);
		void radialStroke(const std::vector<color_t> & ColorTypes, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY);

		void rectMode(DrawMode mode);
		void rect(float x1, float y1, float x2, float y2, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);
		void quad(float x, float y, float size, float cornerRadiusX = 0.f, float cornerRadiusY = 0.f);

		void ellipseMode(DrawMode mode);
		void ellipse(float x, float y, float rx, float ry);
		void circle(float x, float y, float radius);
		void point(float x, float y);

		void line(float x1, float y1, float x2, float y2);
		void imageMode(DrawMode mode);
		void image(const Image & img, float x, float y);
		void image(const Image & img, float x1, float y1, float x2, float y2);

		void beginShape();
		void endShape(ShapeEndType type);
		void shapeFillMode(FillMode fillMode);
		void shapePathSegment(PathSegment pathSegment);

		void vertex(float x, float y);
		void quadraticBezier(float x1, float y1, float x2, float y2);
		void bezier(float x1, float y1, float x2, float y2, float x3, float y3);
		void arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection sweepDirection, ArcSize size);

		void strokeStartCap(CapStyle style);
		void strokeEndCap(CapStyle style);
		void strokeDashCap(CapStyle style);
		void strokeLineJoin(LineJoin lineJoin);
		void strokeDashStyle(DashStyle style);
		void strokeDashes(const std::vector<float> & dashes);
		void strokeDashOffset(float offset);
		void strokeMiterLimit(float limit);

		void text(const std::string & string, float x, float y);
		void text(const std::string & string, float x, float y, float width);
		void text(const std::string & string, float x, float y, float width, float height);

		void textSize(float textSize);
		void textUnderline(bool active);
		void textStrikeThrough(bool active);
		void textFont(const std::string & font);
		void textFontWeight(FontWeight fontWeight);
		void textFontStyle(FontStyle fontStyle);
		void textFontStretch(FontStretch fontStretch);
		void textWordWrapping(WordWrapping wordWrapping);
		void textAlignment(TextAlignment textAlignment);
		void textParagraphAlignment(ParagraphAlignment paragraphAlignment);

		FVector2 getTextSize();
		FVector2 getTextSize(const std::string & content);

		GraphicsContext *& getGraphicsContext() const;

	private:

		GraphicsContext *& gctx;

	};

}