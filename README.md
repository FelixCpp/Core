# Core

A library based on Processing/P5.js to create simple 2D applications in C++



## General informations about Core

This  C++ Project is heavily inspired by [Processing](https://github.com/processing/processing) and [p5.js](https://github.com/processing/p5.js).

It provides several functionalities in these modules:

* Audio
* Maths
* Rendering
* System (stl extensions)
* Window

Core contains fast, richly featured drawing routines and algorithms to

* develop games
* visualize algorithms
* Prototype and experiment
* Educate

This repository is actively maintained and develoed.



## Examples


##### Hello World

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

##### Bouncing Ball

```c++
#include <Core/Application.hpp>
using namespace Core;

class App : public Application {
public:

    float x = 0.f;
    float y = 0.f;
    float xspeed = 3.f;
    float yspeed = 4.f;
    float radius = 50.f;

    App() :
        Application(600, 500, "Bouncing Ball")
    {
    }

protected:

    virtual void Setup() override
    {
        NoStroke();
        Fill(Color::LightRed);

        x = width  / 2;
        y = height / 2;
    }

    virtual void Draw() override
    {
        Background(Color::Black);

        x += xspeed;
        if ((x + radius > width) || (x - radius < 0))
        {
            xspeed *= -1;
        }

        y += yspeed;
        if ((y + radius > height) || (y - radius < 0))
        {
            yspeed *= -1;
        }

        Circle(x, y, radius);
    }
    
};

int main()
{
    Application::Launch<App>();
}
```



##### playing with colors

```c++
...
virtual void Setup() override
{
    NoStroke();
    RadialFill(colors, width / 2, height / 2, 250, 250, 0, 0);

    x = width  / 2;
    y = height / 2;
}
...
```
