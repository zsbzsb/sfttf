#include <sfttf/FontManager.hpp>
#include <sfttf/Font.hpp>
#include <cassert>

sfttf::FontManager::FontManager() : myWindow(0),
									myFreeType(0)
{

}


sfttf::FontManager::~FontManager()
{
	Deinitialize();
}

bool sfttf::FontManager::Initialize(sf::RenderWindow* Window)
{
	Deinitialize();
	if (0 != FT_Init_FreeType(&myFreeType))
		return false;

	myWindow = Window;
	return true;
}

/*
	Deinitialize does two things:
	-Unload fonts
	-Unload FreeType
*/
bool sfttf::FontManager::Deinitialize()
{
	unloadFonts();
	myWindow = 0;

	FT_Library Library = myFreeType;
	myFreeType = 0;
	if (0 != Library)
	{
		if (0 != FT_Done_FreeType(Library))
			return false;
	}
	return true;
}

sfttf::Font* sfttf::FontManager::loadFont(const std::string& Filename)
{
	sfttf::Font* newFont = new sfttf::Font(myWindow, myFreeType);
	if (!newFont->Initialize(Filename))
	{
		delete newFont;
		return 0;
	}
	myFonts.push_back(newFont);
	return newFont;
}

/*
	Note about unloading fonts:
	We do not need to call sfttf::Font::Deinitialize explicitly,
	the font destructor already calls it.
*/
void sfttf::FontManager::unloadFont(sfttf::Font* Font)
{
	FontList::iterator itFont(std::find(myFonts.begin(), myFonts.end(), Font));
	if (myFonts.end() != itFont)
	{
		myFonts.erase(itFont);
		delete Font;
	}
}

void sfttf::FontManager::unloadFonts()
{
	while (!myFonts.empty())
	{
		delete myFonts.back();
		myFonts.pop_back();
	}
}
