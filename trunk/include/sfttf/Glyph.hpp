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
#ifndef SFTTF_GLYPH_HPP
#define SFTTF_GLYPH_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace sfttf
{

struct Glyph
{
	/*
		Actual glyph dimensions, not texture dimensions
	*/
	int glyphWidth;
	int glyphHeight;

	unsigned int glyphIndex;
	int xAdvance;
	int bitmapLeft;
	int bitmapTop;

	sf::Image Texture;
	/*
		We have a few choices here:
		-Use 1 sprite for each Glyph (this is what we do now).
		-Share a static sprite, setting it up each time we draw
		-Use custom drawing code (likely the fastest)
	*/
	sf::Sprite Sprite;

	int Size;
	/*
		Only valid for outline glyphs.
	*/
	int outlineSize;

	bool renderTexture(FT_Bitmap Bitmap);
	void Setup(const FT_Bitmap& Bitmap,
				sf::RenderWindow* theWindow,
				int Left,
				int Top,
				int theXAdvance,
				unsigned int Index,
				int theSize,
				int theOutlineSize);

	bool Draw(float X, float Y, const sf::Color& Color);

private:
	/*
		This is used by renderGlyph/renderGlyphOutline.
		It is only resized when less than the required size.
		It is shared between all Glyph's, and thus not thread-safe.

		Basically it comes down to two methods to create an sf::Image:
		-SetPixel
		-LoadFromPixels

		We're going for LoadFromPixels so we can avoid calling SetPixel
		once per pixel.
		SetPixel doesn't look very expensive (and may be inline-able),
		but this may help anyways.
	*/
	typedef std::vector<sf::Uint8> PixelBuffer;
	static PixelBuffer Pixels;

	sf::RenderWindow* Window;
};

}

#endif
