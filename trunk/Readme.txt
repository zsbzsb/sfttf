Project files are in the build directory.
Included project files:
	Linux:
		codeblocks/gcc
	Windows:
		codeblocks/vc2008
		vc2008


Usage:
Everything lives in the sfttf namespace.
You must create a FontManager to load/use any fonts.

sfttf::FontManager FontMgr;
FontMgr.Initialize();
sfttf::Font* Font = FontMgr.loadFont("Font.ttf");
Font->setSize(22);
Font->drawString("Hello, World!", 5.0f, 22.0f);

When characters are drawn, there are a few factors
that affect the output:
-Kerning	Whether kerning is used (if the font supports it)
-Fill		Fill determines whether a filled (normal) character is drawn
-Size		This is the size of the character in pixels
-Outline	Outline determines whether an outlined character is drawn
-Outline size	This is the size of the outline

-Color		The color of the filled character
-Outline color	The color of the outlined character

Here's an example:
Font->setKerning(true);			//Enable kerning, if supported
Font->setFill(true);			//Draw filled character
Font->setOutline(true);			//Draw outlined character
Font->setSize(72);			//Set the character's size
Font->setOutlineSize(2);		//Set the outline size
Font->setColor(sf::Color::White);	//Set the fill color
Font->setOutlineColor(sf::Color::Blue);	//Set the outline color
Font->drawString("Some text", 100.0f, 100.0f);

It can be quite cumbersome to set all these settings every time.
What if you want to preserve most of them, but change just a few?
Example:
Font->setSize(72);
Font->setColor(sf::Color::White);
...

Font->Push();	//This saves the current settings
Font->setSize(22);
..
Font->drawString("This is size 22", 100.0f, 100.0f);
Font->Pop();	//This restores the current settings

Font->drawString("This is size 72", 100.0f, 200.0f);
