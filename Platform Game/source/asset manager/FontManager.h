#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <string>
#include <map>
#include "Font.h"

#include <ft2build.h>
#include <freetype/freetype.h>

class FontManager
{
public:
	static FontManager &GetInstance() { static FontManager instance; return instance; }
	~FontManager();
	Font &GetFont(const std::string &fontName, unsigned size);
private:
	FontManager();
	FT_Library mFreetypeLib;
	void LoadFontFace(const std::string &fontName);
	void UnloadFontFace(const std::string &fontName);
	void LoadFont(std::string const &fontName, unsigned size);
	std::map<std::string, FT_Face> mFontFaces;
	std::map<std::string, std::map<unsigned,Font>> mFonts;
};

#endif  // FONT_MANAGER_H