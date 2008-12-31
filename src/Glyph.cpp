#include <sfttf/Glyph.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

sfttf::Glyph::PixelBuffer sfttf::Glyph::Pixels;

bool sfttf::Glyph::renderTexture(FT_Bitmap Bitmap)
{
	/*
		We return true here because certain font 'characters'
		will not have a texture but will have other information
		we need.

		A good example is the space character.
	*/
	if (0 == Bitmap.width || 0 == Bitmap.rows)
		return true;

	if (Pixels.size() < static_cast<unsigned int>(Bitmap.rows * Bitmap.width * 4))
		Pixels.resize(Bitmap.rows * Bitmap.width * 4, 0xff);

	unsigned char* pSource = Bitmap.buffer;
	sf::Uint32* pDestination = (sf::Uint32*)&Pixels[0];
	for (int y = 0; y < Bitmap.rows; ++y)
	{
		for (int x = 0; x < Bitmap.width; ++x)
			*(pDestination + x) = 0x00ffffff | *(pSource + x) << 24;

		pDestination += Bitmap.pitch;
		pSource += Bitmap.pitch;
	}
	/*
		If we reuse sprites, this would be required:
		
		Sprite.SetSubRect(sf::IntRect(0, 0, Texture.GetWidth(), Texture.GetHeight()));
	*/
	return Texture.LoadFromPixels(Bitmap.width, Bitmap.rows, (const sf::Uint8*)&Pixels[0]);
}

void sfttf::Glyph::Setup(const FT_Bitmap& Bitmap,
						 sf::RenderWindow* theWindow,
						 int Left,
						 int Top,
						 int theXAdvance,
						 unsigned int Index,
						 int theSize,
						 int theOutlineSize)
{
	glyphWidth = Bitmap.width;
	glyphHeight = Bitmap.rows;
	Window = theWindow;
	bitmapLeft = Left;
	bitmapTop = Top;
	xAdvance = theXAdvance;
	glyphIndex = Index;
	Size = theSize;
	outlineSize = theOutlineSize;

	/*
		Cheap little trick for outline advances.

		When using outlines, the glyphWidth can be greater than the xAdvance.
		This would cause incorrect spacing.
		So we try either xAdvance+outlineSize or glyphWidth+1.
	*/
	if (0 != xAdvance && 0 != outlineSize && xAdvance < glyphWidth)
		xAdvance = std::max(xAdvance + outlineSize, glyphWidth + 1);

	Sprite.SetImage(Texture);
}

/*
	This actually always returns true for now.
*/
bool sfttf::Glyph::Draw(float X, float Y, const sf::Color& Color)
{
	if (0 == glyphWidth || 0 == glyphHeight)
		return true;

	Sprite.SetPosition(sf::Vector2f(X, Y));
	Sprite.SetColor(Color);
	Window->Draw(Sprite);
	return true;
}
