﻿// 
// Library.hpp
// Core
// 
// Created by Felix Busch on 19.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Application/Globals.hpp>

#include <Core/System/Types.hpp>
#include <Core/System/Value2.hpp>
#include <Core/System/String.hpp>

#include <Core/Window/MouseCursor.hpp>

#include <Core/Graphics/RenderStyle.hpp>
#include <Core/Graphics/Color.hpp>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// Forward declarations
	/// 
	////////////////////////////////////////////////////////////
	class Application;
	class Window;
	class GraphicsContext;
	class Shape;

	////////////////////////////////////////////////////////////
	/// Application functions
	/// 
	////////////////////////////////////////////////////////////
	void SetAutoCloseEnabled(bool enabled);
	bool IsAutoCloseEnabled();
	void Exit();
	void SetFrameRateLimit(u32 limit);
	u32 GetFramesPerSecond();
	Application& GetApp();

	////////////////////////////////////////////////////////////
	/// Window functions
	/// 
	////////////////////////////////////////////////////////////
	void SetWindowSize(int32_t width, int32_t height);
	Int2 GetWindowSize();
	void SetWindowPosition(int32_t x, int32_t y);
	Int2 GetWindowPosition();
	void SetWindowTitle(const String& title);
	String GetWindowTitle();
	void SetWindowMaximizable(bool enabled);
	bool IsWindowMaximizable();
	void SetWindowMinimizable(bool enabled);
	bool IsWindowMinimizable();
	void SetWindowResizable(bool enabled);
	bool IsWindowResizable();
	void SetWindowClosable(bool enabled);
	bool IsWindowClosable();
	void RequestWindowFocus();
	bool WindowHasFocus();
	void SetMouseCursorGrabbed(bool grabbed);
	bool IsMouseCursorGrabbed();
	void SetWindowVisible(bool visible);
	bool IsWindowVisible();
	void SetMouseCursorVisible(bool visible);
	bool IsMouseCursorVisible();
	void SetWindowKeyRepeatEnabled(bool enabled);
	bool IsWindowKeyRepeatEnabled();
	void SetMouseCursor(const MouseCursor& cursor);
	const MouseCursor& GetMouseCursor();
	Window& GetWindow();

	////////////////////////////////////////////////////////////
	/// Rendering
	/// 
	////////////////////////////////////////////////////////////
	void Background(const Color& color);
	void Fill(const Color& color);
	void NoFill();
	void Stroke(const Color& color);
	void NoStroke();
	void StrokeWeight(float weight);
	void RectMode(DrawMode mode);
	void Rect(float x1, float y1, float x2, float y2, float cornerX, float cornerY);
	void EllipseMode(DrawMode mode);
	void Ellipse(float a, float b, float c, float d);
	void Line(float x1, float y1, float x2, float y2);
	void StrokeStartCap(CapStyle style);
	void StrokeEndCap(CapStyle style);
	void StrokeDashCap(CapStyle style);
	void StrokeLineJoin(LineJoin join);
	void StrokeMiterLimit(float limit);
	void StrokeDashStyle(DashStyle style);
	void StrokeDashOffset(float offset);
	void StrokeDashes(const float* dashes, usize count);
	void Geometry(const Shape& shape);
	RenderStyle& GetRenderStyle();
	GraphicsContext& GetGraphics();

}