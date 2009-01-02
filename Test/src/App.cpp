#include "App.hpp"
#include <sfttf/Font.hpp>

#ifdef _MSC_VER
#define mysnprintf sprintf_s
#else
#define mysnprintf _snprintf
#endif

/*
	Options:
	ALT+K - toggle kerning
	ALT+F - toggle fill
	ALT+O - toggle outline
	ALT+C - clear cache
	UP - increase font size by 1 or 10 (if CTRL)
	DOWN - decrease font size by 1 or 10 (if CTRL)
	SHIFT+UP - increase font outline by 1 or 10 (if CTRL)
	SHIFT+DOWN - decrease font outline by 1 or 10 (if CTRL)

	Commands:
	ALT+Q - quit
	ESCAPE - quit
	ALT+ENTER - toggle fullscreen
*/

App::App() : myDone(false),
			 myFont(0),
			 myFPS(0.0f),
			 myAverageFPS(0.0f),
			 myLastFPSUpdate(0.0f),
			 myNumberOfFrames(0),
			 myFrameTimes(0.0f),
			 myFullscreen(false)
{

}

void App::Initialize(int argc, char* argv[])
{
	createWindow();
	if (!myFontManager.Initialize(&myWindow))
		throw std::runtime_error("Failed to initialize font manager");

	myFont = myFontManager.loadFont(argc == 2 ? argv[1] : "mona.ttf");
	if (0 == myFont)
		throw std::runtime_error("Failed to load font");

	myFont->setOutline(true);
	myFont->setFill(true);
	myFont->setOutlineColor(sf::Color::White);
	myFont->setColor(sf::Color::Blue);
	myFont->setSize(72);
	myFont->setOutlineSize(1);
}

void App::Run(int argc, char* argv[])
{
	Initialize(argc, argv);
	while (!myDone)
	{
		Update();
		Draw();
		myWindow.Display();
		handleEvents();

		float Delta = myWindow.GetFrameTime();

		myFrameTimes += Delta;
		myLastFPSUpdate += Delta;
		++myNumberOfFrames;
		if (myLastFPSUpdate >= 1.0f)
		{
			myFPS = 1.0f / Delta;
			myAverageFPS = myNumberOfFrames / myFrameTimes;
			myLastFPSUpdate = 0.0f;
		}
	}
}

void App::handleEvents()
{
	sf::Event Event;
	while (myWindow.GetEvent(Event))
	{
		switch (Event.Type)
		{
		case sf::Event::Closed:
			Exit();
			break;
		case sf::Event::KeyPressed:
			onKeyPressed(Event.Key.Code, Event.Key.Alt, Event.Key.Control, Event.Key.Shift);
			break;
		default:
			break;
		}
	}
}

void App::Update()
{

}

void App::Draw()
{
	static char Buffer[256];

	myWindow.Clear();

	static const char* s = "the quick brown fox jumps over the lazy dog";
	float X = 5.0f;
	float Y = 104.0f;
	unsigned int Size = myFont->getSize();
	for (int i = myFont->getSize(); ; ++i)
	{
		myFont->Push();
		myFont->setSize(i);
		myFont->drawString(s, X, Y);
		myFont->Pop();
		Y += i;
		if (Y >= static_cast<float>(ScreenHeight))
			break;
	}

	myFont->Push();
	myFont->setSize(26);
	myFont->setFill(true);
	myFont->setOutline(false);
	myFont->setColor(sf::Color::White);

	mysnprintf(Buffer, sizeof(Buffer), "FPS: %.2f", myFPS);
	myFont->drawString(Buffer, 5, 26);

	mysnprintf(Buffer, sizeof(Buffer), "Avg FPS: %.2f", myAverageFPS);
	myFont->drawString(Buffer, 5, 52);

	mysnprintf(Buffer, sizeof(Buffer), "%dx%d", myWindow.GetInput().GetMouseX(), myWindow.GetInput().GetMouseY());
	myFont->drawString(Buffer, 5, 78);

	mysnprintf(Buffer, sizeof(Buffer), "Size: %d", Size);
	myFont->drawString(Buffer, 5, 104);
	myFont->Pop();
}

void App::onKeyPressed(sf::Key::Code Code, bool Alt, bool Control, bool Shift)
{
	unsigned int Increment = (Control) ? 10 : 1;
	switch (Code)
	{
	case sf::Key::Escape:
		Exit();
		break;
	case sf::Key::Up:
		if (Shift)
			myFont->setOutlineSize(myFont->getOutlineSize() + Increment);
		else
			myFont->setSize(myFont->getSize() + Increment);

		break;
	case sf::Key::Down:
		if (Shift)
			myFont->setOutlineSize(myFont->getOutlineSize() - Increment);
		else
			myFont->setSize(myFont->getSize() - Increment);

		break;
	case sf::Key::C:
		if (Alt)
			myFont->clearCache();

		break;
	case sf::Key::F:
		if (Alt)
			myFont->setFill(!myFont->getFill());

		break;
	case sf::Key::K:
		if (Alt)
			myFont->setKerning(!myFont->getKerning());

		break;
	case sf::Key::O:
		if (Alt)
			myFont->setOutline(!myFont->getOutline());

		break;
	case sf::Key::Q:
		if (Alt)
			Exit();

		break;
	case sf::Key::Return:
		if (Alt)
		{
			myFullscreen = !myFullscreen;
			createWindow();
		}
		break;
	default:
		break;
	}
}

void App::createWindow()
{
	int Flags = sf::Style::Close;
	if (myFullscreen)
		Flags |= sf::Style::Fullscreen;

	myWindow.Create(sf::VideoMode(ScreenWidth, ScreenHeight, 32), "sfttf Test", Flags);
}

void App::Exit()
{
	myWindow.Show(false);
	myDone = true;
}
