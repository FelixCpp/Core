#pragma once

#include <Core/System/StaticObject.hpp>
#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector2.hpp>

namespace Core
{

	struct Mouse {

		enum class Button {
			Left		= 0x01,	//< Left mouse button
			Right		= 0x02,	//< Right mouse button
			Middle		= 0x04,	//< Middle mouse button(three - button mouse)
			XButton1	= 0x05,	//< Windows 2000: X1 mouse button
			XButton2	= 0x06	//< Windows 2000: X2 mouse button
		};

		static bool isButtonPressed(Button button);
		
		static void setPosition(i32_t x, i32_t y);
		static void setPosition(i32_t x, i32_t y, Windowhandle window);

		static void setPosition(const IVector2 & position);
		static void setPosition(const IVector2 & position, Windowhandle window);

		static IVector2 getPosition();
		static IVector2 getPosition(Windowhandle window);

	};

}