#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <sfttf/FontManager.hpp>

const unsigned int ScreenWidth = 800;
const unsigned int ScreenHeight = 600;

class App
{
public:
	App();

	void Run(int argc, char* argv[]);

private:
	bool myDone;
	sf::RenderWindow myWindow;
	sfttf::FontManager myFontManager;
	sfttf::Font* myFont;

	void Initialize(int argc, char* argv[]);
	void createWindow();
	void Update();
	void Draw();
	void handleEvents();
	void Exit();

	void onKeyPressed(sf::Key::Code Code, bool Alt, bool Control, bool Shift);

	float myFPS;
	float myAverageFPS;
	float myLastFPSUpdate;
	unsigned int myNumberOfFrames;
	float myFrameTimes;
	bool myFullscreen;
};

#endif
