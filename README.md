# Core

Lightweight 2D game framework based on Direct2D. Core is in early-stage development phase and will grow from time to time.



#### Getting started Example

*Here's a little demo App to get you started.*

```C++
// 
// Filename: App.cpp
// Project:  Testing
//

#include <Core/Application/Sketch.hpp>
#include <Core/Library.hpp>
#include <Core/Window/MouseCursor.hpp>

#include <iostream>

using namespace Core;

namespace Testing
{
	////////////////////////////////////////////////////////////
	/// \brief Define application implementation callback
	/// 
	////////////////////////////////////////////////////////////
	class App final : public Sketch
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Load image assets
		/// 
		////////////////////////////////////////////////////////////
		virtual bool OnPreload() override
		{
			MouseCursor cursor;
			if(cursor.LoadFromFile("Assets/Cursors/aerowedge.png", UInt2(0, 0)))
			{
				SetMouseCursor(cursor);
			}

			if(!texture.LoadFromFile(L"Assets/Textures/texture.png"))
			{
				return false;
			}
            
			return true;
		}

		////////////////////////////////////////////////////////////
		/// \brief Set up the sketch
		/// 
		////////////////////////////////////////////////////////////
		virtual void OnSetup() override
		{
			SetWindowSize(1280, 720);
		}
		
		////////////////////////////////////////////////////////////
		/// \brief Render content
		/// 
		////////////////////////////////////////////////////////////
		virtual void OnDraw(float deltaTime) override
		{
			Background(Color::Black);
			ImageMode(Center);
			Image(texture, MouseX, MouseY);
		}

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		Texture texture;

	};
}

////////////////////////////////////////////////////////////
/// Tell Core which sketch to launch
/// 
////////////////////////////////////////////////////////////
namespace Core
{
	Sketch* CreateSketch()
	{
		return new Testing::App();
	}
}
```

