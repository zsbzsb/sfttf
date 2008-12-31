#ifndef SFTTF_GLYPH_HPP
#define SFTTF_GLYPH_HPP

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
