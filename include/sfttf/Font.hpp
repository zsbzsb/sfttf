#ifndef SFTTF_FONT_HPP
#define SFTTF_FONT_HPP

#include <sfttf/FontManager.hpp>
#include <map>
#include <vector>
#include FT_STROKER_H

namespace sfttf
{

struct Glyph;
typedef unsigned int uint32_t;

class Font : public sf::NonCopyable
{
public:
	bool cacheChar(uint32_t codePoint);
	bool cacheString(const std::string& s);
	bool cacheString(const std::wstring& s);

	void clearCache();

	void setFill(bool Fill);
	void setOutline(bool Outline);
	void setSize(int Size);
	void setOutlineSize(int Size);

	void setColor(const sf::Color& Color);
	void setOutlineColor(const sf::Color& Color);

	void setKerning(bool Kerning);

	bool drawChar(uint32_t codePoint, float X, float Y);
	bool drawString(const std::string& s, float X, float Y);
	bool drawString(const std::wstring& s, float X, float Y);

	unsigned int getCharWidth(uint32_t codePoint);
	unsigned int getCharHeight(uint32_t codePoint);

	unsigned int getStringWidth(const std::string& s);
	unsigned int getStringWidth(const std::wstring& s);
	unsigned int getStringHeight(const std::string& s);
	unsigned int getStringHeight(const std::wstring& s);

	bool getFill() const;
	bool getOutline() const;

	int getSize() const;
	int getOutlineSize() const;

	bool getKerning() const;

	void Push();
	void Pop();

private:
	/*
	Quick helper class to insure FT_Done_Glyph is called on an FT_Glyph
	*/
	struct FreeGlyph
	{
		FreeGlyph(FT_Glyph* Glyph) : myGlyph(Glyph)
		{}

		~FreeGlyph()
		{
			if (0 != myGlyph)
				FT_Done_Glyph(*myGlyph);
		}

	private:
		FT_Glyph* myGlyph;
	};
	/*
		All client-writeable settings.
		They're in a separate struct so we can easily make a settings 'stack'.
		See Push/Pop
	*/
	struct Settings
	{
		Settings() : Fill(true),
					 Outline(false),
					 Size(32),
					 outlineSize(1),
					 Kerning(true),
					 Color(sf::Color::White),
					 outlineColor(sf::Color::Blue)
		{}
		bool Fill;
		bool Outline;
		int Size;
		int outlineSize;

		bool Kerning;
		sf::Color Color;
		sf::Color outlineColor;
	};
	typedef std::multimap<uint32_t, Glyph*> GlyphCache;
	typedef std::pair<GlyphCache::iterator, GlyphCache::iterator> GlyphRange;

	typedef std::vector<Settings> SettingsStack;

	Font(sf::RenderWindow* Window, FT_Library FreeType);
	~Font();

	bool Initialize(const std::string& Filename);
	void Deinitialize();

	bool updateSize();
	void updateOutlineSize();

	bool cacheGlyph(uint32_t codePoint);
	bool isGlyphCached(uint32_t codePoint);
	bool cacheGlyphOutline(uint32_t codePoint);
	bool isGlyphOutlineCached(uint32_t codePoint);

	Glyph* getGlyph(uint32_t codePoint);
	Glyph* getGlyphOutline(uint32_t codePoint);

	Glyph* drawChar(uint32_t codePoint, float& penX, float& penY, Glyph* previousGlyph);
	void getKerning(Glyph* leftGlyph, Glyph* rightGlyph, int* offsetX, int* offsetY);

	template <typename TIterator>
	bool cacheString(TIterator itCodePoint, const TIterator& itEnd);

	template <typename TIterator>
	bool drawString(TIterator itCodePoint, const TIterator& itEnd, float X, float Y);

	template <typename TIterator>
	unsigned int getStringWidth(TIterator itCodePoint, const TIterator& itEnd);

	template <typename TIterator>
	unsigned int getStringHeight(TIterator itCodePoint, const TIterator& itEnd);

	friend class FontManager;

	sf::RenderWindow* myWindow;
	FT_Library myFreeType;
	FT_Face myFace;
	FT_Stroker myStroker;

	Settings mySettings;
	SettingsStack mySettingsStack;

	int myFreeTypeSize;

	int myFreeTypeOutlineSize;

	bool myHaveKerning;

	GlyphCache myGlyphs;
	GlyphCache myGlyphOutlines;
};

}

#endif
