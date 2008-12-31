/**********************************************************************
Copyright (c) 2008, Daniel Wyatt (Daniel.Wyatt@gmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********************************************************************/
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
