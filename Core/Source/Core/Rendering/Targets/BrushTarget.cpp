#include <Core/Rendering/Targets/BrushTarget.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/RenderState.hpp>

namespace Core
{

	BrushTarget::BrushTarget(LateRef<RenderStateManager> rsm) :
		rsm(rsm)
	{
	}

	void BrushTarget::StrokeWeight(float strokeWeight)
	{
		this->GetState().strokeWeight = strokeWeight;
	}

	void BrushTarget::NoFill()
	{
		this->GetState().activeFill = nullptr;
	}

	void BrushTarget::NoStroke()
	{
		this->GetState().activeStroke = nullptr;
	}

	void BrushTarget::Fill(const Color & color)
	{
		RenderState & state = this->GetState();
		state.solidFill.SetColor(color);
		state.activeFill = state.solidFill.GetBrush();
	}

	void BrushTarget::Stroke(const Color & color)
	{
		RenderState & state = this->GetState();
		state.solidStroke.SetColor(color);
		state.activeStroke = state.solidStroke.GetBrush();
	}

	void BrushTarget::LinearFill(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->GetState();
		state.linearFill.SetStart(startX, startY);
		state.linearFill.SetEnd(endX, endY);
		state.linearFill.SetColors(colors);
		state.activeFill = state.linearFill.GetBrush();
	}

	void BrushTarget::LinearStroke(const std::vector<Color> & colors, float startX, float startY, float endX, float endY)
	{
		RenderState & state = this->GetState();
		state.linearStroke.SetStart(startX, startY);
		state.linearStroke.SetEnd(endX, endY);
		state.linearStroke.SetColors(colors);
		state.activeStroke = state.linearStroke.GetBrush();
	}

	void BrushTarget::RadialFill(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->GetState();
		state.radialFill.SetCenter(centerX, centerY);
		state.radialFill.SetRadius(radiusX, radiusY);
		state.radialFill.SetOffset(offsetX, offsetY);
		state.radialFill.SetColors(colors);
		state.activeFill = state.radialFill.GetBrush();
	}

	void BrushTarget::RadialStroke(const std::vector<Color> & colors, float centerX, float centerY, float radiusX, float radiusY, float offsetX, float offsetY)
	{
		RenderState & state = this->GetState();
		state.radialStroke.SetCenter(centerX, centerY);
		state.radialStroke.SetRadius(radiusX, radiusY);
		state.radialStroke.SetOffset(offsetX, offsetY);
		state.radialStroke.SetColors(colors);
		state.activeStroke = state.radialStroke.GetBrush();
	}

	void BrushTarget::ImageFill(const Image & image, ImageInterpolationMode mode, ExtendMode modeX, ExtendMode modeY, i32_t opacity)
	{
		RenderState & state = this->GetState();
		state.bitmapFill.SetImage(image);
		state.bitmapFill.SetInterpolationMode(mode);
		state.bitmapFill.SetExtendModeX(modeX);
		state.bitmapFill.SetExtendModeY(modeY);
		state.activeFill = state.bitmapFill.GetBrush();
	}

	void BrushTarget::ImageStroke(const Image & image, ImageInterpolationMode mode, ExtendMode modeX, ExtendMode modeY, i32_t opacity)
	{
		RenderState & state = this->GetState();
		state.bitmapStroke.SetImage(image);
		state.bitmapStroke.SetInterpolationMode(mode);
		state.bitmapStroke.SetExtendModeX(modeX);
		state.bitmapStroke.SetExtendModeY(modeY);
		state.activeStroke = state.bitmapStroke.GetBrush();
	}

	RenderState & BrushTarget::GetState()
	{
		return this->rsm->GetActiveState();
	}

}