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
