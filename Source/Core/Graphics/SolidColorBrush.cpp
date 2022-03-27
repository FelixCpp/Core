// 
// SolidColorBrush.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/SolidColorBrush.hpp>
#include <Core/Application/Application.hpp>
#include <Core/Library.hpp>
#include <wrl/client.h>
#include <d2d1.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define concrete implementation of the brush.
	/// 
	////////////////////////////////////////////////////////////
	class SolidColorBrush::Impl
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Change the brush's color.
		/// 
		////////////////////////////////////////////////////////////
		void SetColor(const Color& color)
		{
			const D2D1_COLOR_F d2dColor = D2D1::ColorF(
				(float)color.R / 255.0f,
				(float)color.G / 255.0f,
				(float)color.B / 255.0f,
				(float)color.A / 255.0f
			);

			if (brush == nullptr)
			{
				GetGraphics().GetRenderTarget().CreateSolidColorBrush(d2dColor, &brush);
			} else
			{
				brush->SetColor(d2dColor);
			}
		}

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying brush.
		/// 
		////////////////////////////////////////////////////////////
		ID2D1Brush* GetBrush() const
		{
			return brush.Get();
		}

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;	///< The wrapped brush

	};

	////////////////////////////////////////////////////////////
	SolidColorBrush::SolidColorBrush():
		impl(std::make_shared<Impl>()),
		color(Color::White)
	{
	}

	////////////////////////////////////////////////////////////
	void SolidColorBrush::SetColor(const Color& color)
	{
		this->color = color;
		impl->SetColor(color);
	}

	////////////////////////////////////////////////////////////
	const Color& SolidColorBrush::GetColor() const
	{
		return color;
	}

	////////////////////////////////////////////////////////////
	ID2D1Brush* SolidColorBrush::GetBrush() const
	{
		return impl->GetBrush();
	}
}
