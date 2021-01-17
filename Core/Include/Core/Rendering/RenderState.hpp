#pragma once

#include <Core/Rendering/DrawMode.hpp>
#include <Core/Rendering/StrokeStyle.hpp>
#include <Core/Rendering/TextRenderer.hpp>
#include <Core/Rendering/Shape.hpp>
#include <Core/Rendering/SolidColorBrush.hpp>
#include <Core/Rendering/LinearGradientBrush.hpp>
#include <Core/Rendering/RadialGradientBrush.hpp>
#include <Core/Rendering/BitmapBrush.hpp>

#include <d2d1.h>
#include <stack>

namespace Core
{

	class GraphicsContext;

	class RenderState {
	public:

		/* manages the shape rendering */
		Shape shape;

		/* manages the text rendering */
		TextRenderer textRenderer;

		/* manages the stroke style rendering */
		StrokeStyle strokeStyle;

		/* this brush-pair should be used to draw anything on screen */
		ID2D1Brush * activeFill, * activeStroke;

		/* this brush-pair holds the solid color brush */
		SolidColorBrush solidFill, solidStroke;

		/* this brush-pair holds the linear color brush */
		LinearGradientBrush linearFill, linearStroke;
		
		/* this brush-pair holds the radial color brush */
		RadialGradientBrush radialFill, radialStroke;

		/* this brush-pair holds the a brush managed by bitmaps */
		BitmapBrush bitmapFill, bitmapStroke;

		/* the current rectangle mode */
		DrawMode rectMode;

		/* the current ellipse mode */
		DrawMode ellipseMode;

		/* the current image mode */
		DrawMode imageMode;

		/* the current stroke weight (outline thickness) */
		float strokeWeight;

		/* constructor */
		explicit RenderState(GraphicsContext *& gctx);

		/* changes the active matrix to match the parameter. This method also calls activateMatrix() */
		void SetActiveMatrix(const D2D1::Matrix3x2F & matrix);

		/*
			If the stack of metrics is empty,
			this method returns the 'defaultMatrix'.
			Otherwise it will return the highest element
			of the stack
		*/
		D2D1::Matrix3x2F & GetActiveMatrix();

		/*
			Pushes another D2D1::Matrix3x2F element
			onto the stack and calls activateMatrix()
			so it gets activates using the RenderTarget
		*/
		void PushMatrix();

		/*
			Pops the highest element from metrics
			and activates the next matrix
		*/
		void PopMatrix();

		/*
			simply pops every matrix from the stack
			and activates the defaultMatrix
		*/
		void Reset();

		/*
			activates the RenderState.
			It means to tell the RenderTarget which matrix to use
		*/
		void ActivateMatrix();

	private:

		/* the default matrix. For more: see documentation getActiveMatrix() */
		D2D1::Matrix3x2F defaultMatrix;

		/* the currently used matrix */
		D2D1::Matrix3x2F * activeMatrix;

		/* a stack of metrics */
		std::stack<D2D1::Matrix3x2F> metrics;

	private:

		/*
			A GraphicsContext to keep track of since passing it into the constructor.
			Through this instance we got access to the RenderTarget to change the
			Transformation
		*/
		GraphicsContext *& gctx;

	};

}