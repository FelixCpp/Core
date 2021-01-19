# Core

A library based on Processing/P5.js to create simple 2D applications in C++

## General informations about Core

This  C++ Project is heavily inspired by [Processing](https://github.com/processing/processing) and [p5.js](https://github.com/processing/p5.js).

It provides several functionalities in these modules:

* Audio
* Maths
* Rendering
* System
* Window

Core contains fast, richly featured drawing routines and algorithms to

* develop games
* visualize algorithms
* Prototype and experiment
* Educate

This repository is actively maintained and develoed.

## Code example

``` C++
#include <Core/Application.hpp>
using namespace Core;

class App : public Application {
public:

	App() :
		Application(400, 400, "Hello World App")
	{ }

protected:

	virtual void Setup() override
	{
		TextSize(42.f);
		NoStroke();
		Fill(Color::White);
	}

	virtual void Draw() override
	{
		Background(Color::Black);
		Text("Hello World", 0.f, 0.f, (float)width, (float)height);
	}

};

int main()
{
    Application::Launch<App>();
}
```

## For more informations / code examples click [here](https://github.com/FelixCpp/Core/wiki)
