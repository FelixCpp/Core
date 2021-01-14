#pragma once

#include <Core/System/StaticObject.hpp>
#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector2.hpp>

#include <vector>

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

		/* returns true if the passed button is currently being pressed */
		static bool IsButtonPressed(Button button);
		
		/* checks every button state. If the button is pressed, he will be stored and returned */
		static std::vector<Button> AreButtonsPressed(const std::vector<Button> & buttons);

		/* changes the cursors position based on the monitors 0, 0 location */
		static void SetPosition(i32_t x, i32_t y);
		
		/* changes the cursors position based on the windows 0, 0 location */
		static void SetPosition(i32_t x, i32_t y, Windowhandle window);

		/* changes the cursors position based on the monitors 0, 0 location */
		static void SetPosition(const IVector2 & position);

		/* changes the cursors position based on the windows 0, 0 location */
		static void SetPosition(const IVector2 & position, Windowhandle window);

		/* returns the cursors position based on the monitors 0, 0 location */
		static IVector2 GetPosition();

		/* returns the cursors position based on the windows 0, 0 location */
		static IVector2 GetPosition(Windowhandle window);

	};

}