#ifndef SFTTF_FONT_MANAGER_HPP
#define SFTTF_FONT_MANAGER_HPP

#include <list>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace sfttf
{

class Font;
class FontManager : public sf::NonCopyable
{
public:
	FontManager();
	~FontManager();

	/*
		This must be explicitly called prior to use!

		Calling it multiple times will cause it to Deinitialize and Reinitialize,
		unloading all fonts in the process.
	*/
	bool Initialize(sf::RenderWindow* Window);
	/*
		This does not need to be called explicitly, it will be called in the destructor.
		However, you are free to call it.
		It will unload all fonts, thus your font pointers will be invalid.
		You must call Initialize again before using it.
	*/
	bool Deinitialize();

	/*
		Load a font.

		Returns the font handle or 0 if it failed.
	*/
	sfttf::Font* loadFont(const std::string& Filename);

	/*
		Unload a font.

		If the font does not belong to this font manager, this call
		does nothing.

		Otherwise, after this call the pointer is invalid.
	*/
	void unloadFont(sfttf::Font* Font);

	/*
		Unload all fonts.
	*/
	void unloadFonts();

private:
	sf::RenderWindow* myWindow;
	FT_Library myFreeType;

	typedef std::list<sfttf::Font*> FontList;
	FontList myFonts;

};

}

#endif
