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
#include <sfttf/Font.hpp>
#include <sfttf/Glyph.hpp>
#include <cassert>
#include <cwctype>

/*
	TODO:
	There is a small bug with kerning.
	When kerning is enabled, you can see rendered text 'jump' the first time
	it is rendered.
*/

/*
	NOTES:
	-In this code, a 'char' is a combination of a filled glyph and/or outline
	-A glyph is either a regular filled glyph or an outline
	-Functions are in the same order as they are in the header
*/

bool sfttf::Font::cacheChar(uint32_t codePoint)
{
	bool Result = true;
	if (mySettings.Fill && !cacheGlyph(codePoint))
		Result = false;

	if (mySettings.Outline && 0 != mySettings.outlineSize && !cacheGlyphOutline(codePoint))
		Result = false;

	return Result;
}

bool sfttf::Font::cacheString(const std::string& s)
{
	return cacheString(s.begin(), s.end());
}

bool sfttf::Font::cacheString(const std::wstring& s)
{
	return cacheString(s.begin(), s.end());
}

void sfttf::Font::clearCache()
{
/*
	TODO: Which is fastest?
*/
#if 0
	GlyphCache::iterator itGlyph(myGlyphs.begin());
	GlyphCache::iterator itEnd(myGlyphs.end());
	for (; itEnd != itGlyph; ++itGlyph)
		delete itGlyph->second;

	myGlyphs.clear();

	itGlyph = myGlyphOutlines.begin();
	itEnd = myGlyphOutlines.end();
	for (; itEnd != itGlyph; ++itGlyph)
		delete itGlyph->second;

	myGlyphOutlines.clear();
#else
	while (!myGlyphs.empty())
	{
		delete myGlyphs.begin()->second;
		myGlyphs.erase(myGlyphs.begin());
	}
	while (!myGlyphOutlines.empty())
	{
		delete myGlyphOutlines.begin()->second;
		myGlyphOutlines.erase(myGlyphOutlines.begin());
	}
#endif
}

/*
	If Fill is true, the filled glyph will be cached/drawn.
*/
void sfttf::Font::setFill(bool Fill)
{
	mySettings.Fill = Fill;
}

/*
	If Outline is true, the glyph outline will be cached/drawn
*/
void sfttf::Font::setOutline(bool Outline)
{
	mySettings.Outline = Outline;
}

/*
When setSize is called, mySize is simply changed.

myFreeTypeSize is the size last set with FT_Set_Pixel_Sizes.
To avoid needlessly calling FT_Set_Pixel_Sizes, it is called
only if mySize != myFreeTypeSize, when a glyph is cached.
*/
void sfttf::Font::setSize(int Size)
{
	mySettings.Size = std::abs(Size);
}

void sfttf::Font::setOutlineSize(int Size)
{
	mySettings.outlineSize = std::abs(Size);
}

void sfttf::Font::setColor(const sf::Color& Color)
{
	mySettings.Color = Color;
}

void sfttf::Font::setOutlineColor(const sf::Color& Color)
{
	mySettings.outlineColor = Color;
}

void sfttf::Font::setKerning(bool Kerning)
{
	mySettings.Kerning = Kerning;
}

bool sfttf::Font::drawChar(uint32_t codePoint, float X, float Y)
{
	sfttf::Glyph* filledGlyph = 0;
	sfttf::Glyph* outlinedGlyph = 0;
	if (cacheChar(codePoint))
	{
		if (mySettings.Fill)
		{
			filledGlyph = getGlyph(codePoint);
			if (!filledGlyph->Draw(X + static_cast<float>(filledGlyph->bitmapLeft), Y - static_cast<float>(filledGlyph->bitmapTop), mySettings.Color))
				return false;
		}
		if (mySettings.Outline && 0 != mySettings.outlineSize)
		{
			outlinedGlyph = getGlyphOutline(codePoint);
			if (!outlinedGlyph->Draw(X + static_cast<float>(outlinedGlyph->bitmapLeft), Y - static_cast<float>(outlinedGlyph->bitmapTop), mySettings.outlineColor))
				return false;
		}
	}
	return filledGlyph || outlinedGlyph;
}

bool sfttf::Font::drawString(const std::string& s, float X, float Y)
{
	return drawString(s.begin(), s.end(), X, Y);
}

bool sfttf::Font::drawString(const std::wstring& s, float X, float Y)
{
	return drawString(s.begin(), s.end(), X, Y);
}

unsigned int sfttf::Font::getCharWidth(uint32_t codePoint)
{
	Glyph* g = 0;
	if (cacheChar(codePoint))
	{
		/*
			Prefer outline as it should be larger
		*/
		if (mySettings.Outline && 0 != mySettings.outlineSize)
			g = getGlyphOutline(codePoint);
		else if (mySettings.Fill)
			g = getGlyph(codePoint);
	}
	return g ? g->glyphWidth : 0;
}

unsigned int sfttf::Font::getCharHeight(uint32_t codePoint)
{
	Glyph* g = 0;
	if (cacheChar(codePoint))
	{
		/*
			Prefer outline as it should be larger
		*/
		if (mySettings.Outline && 0 != mySettings.outlineSize)
			g = getGlyphOutline(codePoint);
		else if (mySettings.Fill)
			g = getGlyph(codePoint);
	}
	return g ? g->glyphHeight : 0;
}

unsigned int sfttf::Font::getStringWidth(const std::string& s)
{
	return getStringWidth(s.begin(), s.end());
}

unsigned int sfttf::Font::getStringWidth(const std::wstring& s)
{
	return getStringWidth(s.begin(), s.end());
}

unsigned int sfttf::Font::getStringHeight(const std::string& s)
{
	return getStringHeight(s.begin(), s.end());
}

unsigned int sfttf::Font::getStringHeight(const std::wstring& s)
{
	return getStringHeight(s.begin(), s.end());
}

bool sfttf::Font::getFill() const
{
	return mySettings.Fill;
}

bool sfttf::Font::getOutline() const
{
	return mySettings.Outline;
}

int sfttf::Font::getSize() const
{
	return mySettings.Size;
}

int sfttf::Font::getOutlineSize() const
{
	return mySettings.outlineSize;
}

bool sfttf::Font::getKerning() const
{
	return mySettings.Kerning;
}

void sfttf::Font::Push()
{
	mySettingsStack.push_back(mySettings);
}

void sfttf::Font::Pop()
{
	if (mySettingsStack.empty())
		return;

	mySettings = mySettingsStack.back();
	mySettingsStack.pop_back();
}

sfttf::Font::Font(sf::RenderWindow* Window, FT_Library FreeType) : myWindow(Window),
																   myFreeType(FreeType),
																   myFace(0),
																   myStroker(0),
																   myFreeTypeSize(0),
																   myFreeTypeOutlineSize(0),
																   myHaveKerning(false)
{
}

sfttf::Font::~Font()
{
	Deinitialize();
}

bool sfttf::Font::Initialize(const std::string& Filename)
{
	if (0 != FT_New_Face(myFreeType, Filename.c_str(), 0, &myFace))
		return false;

	/*
	Only scalable Unicode fonts (I have not tested anything else)
	*/
	if (!myFace->charmap || !FT_IS_SCALABLE(myFace))
	{
		Deinitialize();
		return false;
	}
	if (0 != FT_Stroker_New(myFreeType, &myStroker))
	{
		Deinitialize();
		return false;
	}
	if (FT_HAS_KERNING(myFace))
		myHaveKerning = true;

	return true;
}

void sfttf::Font::Deinitialize()
{
	if (0 != myFace)
		assert(0 == FT_Done_Face(myFace));

	myFace = 0;
}

/*
This is the function that actually calls FT_Set_Pixel_Sizes
*/
bool sfttf::Font::updateSize()
{
	if (mySettings.Size != myFreeTypeSize)
	{
		if (0 != FT_Set_Pixel_Sizes(myFace, mySettings.Size, 0))
			return false;

		myFreeTypeSize = mySettings.Size;
	}
	return true;
}

void sfttf::Font::updateOutlineSize()
{
	if (mySettings.outlineSize != myFreeTypeOutlineSize)
	{
		FT_Stroker_Set(myStroker,
			64 * mySettings.outlineSize,
			FT_STROKER_LINECAP_ROUND,
			FT_STROKER_LINEJOIN_ROUND,
			0);
	}
}

bool sfttf::Font::cacheGlyph(uint32_t codePoint)
{
	if (0 == mySettings.Size)
		return false;

	if (isGlyphCached(codePoint))
		return true;

	if (!updateSize())
		return false;

	FT_UInt glyphIndex = FT_Get_Char_Index(myFace, codePoint);
	/*
	You have two choices for glyphIndex == 0 (code point not in font):
	-Return false
	-Ignore it. This will typically show a 'box'

	if (0 == glyphIndex)
		return false;
	*/

	if (0 != FT_Load_Glyph(myFace, glyphIndex, FT_LOAD_RENDER))
		return false;

	Glyph* newGlyph = new Glyph;
	FT_Bitmap& Bitmap = myFace->glyph->bitmap;
	if (!newGlyph->renderTexture(Bitmap))
	{
		delete newGlyph;
		return false;
	}
	newGlyph->Setup(Bitmap,
		myWindow,
		myFace->glyph->bitmap_left,
		myFace->glyph->bitmap_top,
		myFace->glyph->metrics.horiAdvance >> 6,
		glyphIndex,
		mySettings.Size,
		0);

	myGlyphs.insert(GlyphCache::value_type(codePoint, newGlyph));
	return true;
}

bool sfttf::Font::isGlyphCached(uint32_t codePoint)
{
	return 0 != getGlyph(codePoint);
}

bool sfttf::Font::cacheGlyphOutline(uint32_t codePoint)
{
	if (0 == mySettings.Size || 0 == mySettings.outlineSize)
		return false;

	if (isGlyphOutlineCached(codePoint))
		return true;

	if (!updateSize())
		return false;

	updateOutlineSize();
	FT_UInt glyphIndex = FT_Get_Char_Index(myFace, codePoint);
	/*
	You have two choices for glyphIndex == 0 (code point not in font):
	-Return false
	-Ignore it. This will typically show a 'box'

	if (0 == glyphIndex)
		return false;
	*/

	if (0 != FT_Load_Glyph(myFace, glyphIndex, FT_LOAD_NO_BITMAP))
		return false;

	if (!(myFace->glyph->format & FT_GLYPH_FORMAT_OUTLINE))
		return false;

	FT_Glyph Glyph = 0;
	if (0 != FT_Get_Glyph(myFace->glyph, &Glyph))
		return false;

	/*
	This will insure FT_Done_Glyph is called.
	FT_Glyph is a pointer.
	FT_Glyph_Stroke/FT_Glyph_To_Bitmap will actually change where
	the FT_Glyph points, and (optionally) call FT_Done_Glyph on the old one.
	This is why FreeGlyph must take the address of the FT_Glyph.
	Otherwise, we could leak or FT_Done_Glyph could be called twice (crash).
	*/
	FreeGlyph f(&Glyph);
	if (0 != FT_Glyph_Stroke(&Glyph, myStroker, 1))
		return false;

	if (0 != FT_Glyph_To_Bitmap(&Glyph, FT_RENDER_MODE_NORMAL, 0, 1))
		return false;

	/*
	See the FreeType docs/demos.
	*/
	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(Glyph);
	sfttf::Glyph* newGlyphOutline = new sfttf::Glyph;
	FT_Bitmap& Bitmap = bitmapGlyph->bitmap;
	if (!newGlyphOutline->renderTexture(Bitmap))
	{
		delete newGlyphOutline;
		return false;
	}
	newGlyphOutline->Setup(Bitmap,
		myWindow,
		bitmapGlyph->left,
		bitmapGlyph->top,
		Glyph->advance.x >> 16,
		glyphIndex,
		mySettings.Size,
		mySettings.outlineSize);

	myGlyphOutlines.insert(GlyphCache::value_type(codePoint, newGlyphOutline));
	return true;
}

bool sfttf::Font::isGlyphOutlineCached(uint32_t codePoint)
{
	return 0 != getGlyphOutline(codePoint);
}

sfttf::Glyph* sfttf::Font::getGlyph(uint32_t codePoint)
{
	GlyphRange Range(myGlyphs.equal_range(codePoint));
	GlyphCache::iterator itGlyph(Range.first);
	const GlyphCache::const_iterator itEnd(Range.second);

	for (; itEnd != itGlyph; ++itGlyph)
	{
		Glyph* g = itGlyph->second;
		if (g->Size == mySettings.Size)
			return g;
	}
	return 0;
}

sfttf::Glyph* sfttf::Font::getGlyphOutline(uint32_t codePoint)
{
	GlyphRange Range(myGlyphOutlines.equal_range(codePoint));
	GlyphCache::iterator itGlyphOutline(Range.first);
	const GlyphCache::const_iterator itEnd(Range.second);

	for (; itEnd != itGlyphOutline; ++itGlyphOutline)
	{
		Glyph* g = itGlyphOutline->second;
		if (g->Size == mySettings.Size && g->outlineSize == mySettings.outlineSize)
			return g;
	}
	return 0;
}

sfttf::Glyph* sfttf::Font::drawChar(uint32_t codePoint, float& penX, float& penY, Glyph* previousGlyph)
{
	sfttf::Glyph* filledGlyph = 0;
	sfttf::Glyph* outlinedGlyph = 0;
	if (cacheChar(codePoint))
	{
		if (mySettings.Fill)
			filledGlyph = getGlyph(codePoint);

		if (mySettings.Outline && 0 != mySettings.outlineSize)
			outlinedGlyph = getGlyphOutline(codePoint);

		int kerningX = 0;
		int kerningY = 0;
		if (filledGlyph)
			getKerning(previousGlyph, filledGlyph, &kerningX, &kerningY);
		else if (outlinedGlyph)
			getKerning(previousGlyph, outlinedGlyph, &kerningX, &kerningY);

		penX += static_cast<float>(kerningX);
		penY += static_cast<float>(kerningY);
		if (filledGlyph)
		{
			if (!filledGlyph->Draw(penX + static_cast<float>(filledGlyph->bitmapLeft), penY - static_cast<float>(filledGlyph->bitmapTop), mySettings.Color))
				return 0;
		}
		if (outlinedGlyph)
		{
			if (!outlinedGlyph->Draw(penX + static_cast<float>(outlinedGlyph->bitmapLeft), penY - outlinedGlyph->bitmapTop, mySettings.outlineColor))
				return 0;
		}

		if (outlinedGlyph)
			penX += static_cast<float>(outlinedGlyph->xAdvance);
		else if (filledGlyph)
			penX += static_cast<float>(filledGlyph->xAdvance);
	}
	return outlinedGlyph ? outlinedGlyph : filledGlyph;
}

/*
	This gets the relative offsets for kerning.
*/
void sfttf::Font::getKerning(Glyph* leftGlyph, Glyph* rightGlyph, int* offsetX, int* offsetY)
{
	FT_Vector Delta;
	if (offsetX)
		*offsetX = 0;

	if (offsetY)
		*offsetY = 0;

	if (myHaveKerning && mySettings.Kerning && leftGlyph && rightGlyph)
	{
		if (0 == FT_Get_Kerning(myFace, leftGlyph->glyphIndex, rightGlyph->glyphIndex, FT_KERNING_DEFAULT, &Delta))
		{
			if (offsetX)
				*offsetX = Delta.x >> 6;

			if (offsetY)
				*offsetY = Delta.y >> 6;
		}
	}
}

template <typename TIterator>
bool sfttf::Font::cacheString(TIterator itCodePoint, const TIterator& itEnd)
{
	bool Result = false;
	for (; itEnd != itCodePoint; ++itCodePoint)
	{
		if (!cacheChar(*itCodePoint))
			Result = false;
	}
	return Result;
}

template <typename TIterator>
bool sfttf::Font::drawString(TIterator itCodePoint, const TIterator& itEnd, float X, float Y)
{
	float penX = X;
	float penY = Y;
	bool Result = true;
	uint32_t codePoint = 0;
	Glyph* g = 0;
	Glyph* previousGlyph = 0;
	for (; itEnd != itCodePoint; ++itCodePoint)
	{
		codePoint = *itCodePoint;
		g = drawChar(codePoint, penX, penY, previousGlyph);
		if (0 == g)
			Result = false;

		previousGlyph = g;
	}
	return Result;
}

template <typename TIterator>
unsigned int sfttf::Font::getStringWidth(TIterator itCodePoint, const TIterator& itEnd)
{
	uint32_t codePoint = 0;
	unsigned int Width = 0;
	Glyph* g = 0;
	Glyph* previousGlyph = 0;
	for (; itEnd != itCodePoint; ++itCodePoint)
	{
		codePoint = *itCodePoint;
		if (!cacheChar(codePoint))
			continue;

		if (mySettings.Outline && 0 != mySettings.outlineSize)
			g = getGlyphOutline(codePoint);
		else
			g = getGlyph(codePoint);

		int kerningX;
		getKerning(previousGlyph, g, &kerningX, 0);

		Width += g->xAdvance + kerningX;
		previousGlyph = g;
	}
	if (g)
	{
		Width -= g->xAdvance;
		Width += g->glyphWidth;
	}
	return Width;
}

template <typename TIterator>
unsigned int sfttf::Font::getStringHeight(TIterator itCodePoint, const TIterator& itEnd)
{
	uint32_t codePoint = 0;
	Glyph* g = 0;
	unsigned int Height = 0;
	for (; itEnd != itCodePoint; ++itCodePoint)
	{
		codePoint = *itCodePoint;
		if (!cacheChar(codePoint))
			continue;

		if (mySettings.Outline && 0 != mySettings.outlineSize)
			g = getGlyphOutline(codePoint);
		else
			g = getGlyph(codePoint);

		if (static_cast<unsigned int>(g->glyphHeight) > Height)
			Height = g->glyphHeight;
	}
	return Height;
}
