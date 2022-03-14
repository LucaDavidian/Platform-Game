#include "FontManager.h"
#include "../logger/Logger.h"
#include "Texture.h"
#include "../data structures/vector.hpp"

FontManager::FontManager()
{
	if (FT_Init_FreeType(&mFreetypeLib) != FT_Err_Ok)  // FT_Err_OK == 0 
		Logger::Instance().Log("Error: cannot initialize freetype library", Logger::Level::LOG_ERROR);
}

FontManager::~FontManager()
{
	for (const std::pair<std::string,FT_Face> &fontFace : mFontFaces)
		FT_Done_Face(fontFace.second);

	FT_Done_FreeType(mFreetypeLib);
}

Font &FontManager::GetFont(const std::string &fontName, unsigned size)
{
	if (mFontFaces.find(fontName) == mFontFaces.end())
		LoadFontFace(fontName);

	if (mFonts.find(fontName) == mFonts.end())
		LoadFont(fontName, size);
	else if (mFonts[fontName].find(size) == mFonts[fontName].end())
		LoadFont(fontName, size);

	return mFonts[fontName].at(size);
}

void FontManager::LoadFontFace(const std::string &fontName)
{
	FT_Face face;  // a face describes a typeface and style (font family + style)

	std::string path = "assets/fonts/" + fontName;

	//if (FT_New_Face(mFreetypeLib, path.c_str(), -1, NULL))   // setting the face index as -1 returns the number of font faces
	//{
	//	LogError("Error: could not open font");
	//	Exit();
	//}

	//long int numFaces = face->num_faces;   // get the number of font faces

	if (FT_New_Face(mFreetypeLib, path.c_str(), 0, &face) != FT_Err_Ok)  // 0 is the font face index (some font files can contain more than one font face)
		Logger::Instance().Log("Error: could not open font", Logger::Level::LOG_ERROR);

	mFontFaces.insert(std::pair<std::string, FT_Face>(fontName, face));  

	//if (face->face_flags & FT_FACE_FLAG_SCALABLE)              // font face properties
	//	Log(std::string(face->family_name) + " is scalable");

	FT_Size faceSize = face->size;   // size of font face

	//FT_CharMap faceCharacterMap = face->charmap;           // current character map (Unicode is default)
	//int numCharacterMaps = face->num_charmaps;             // number of available charmaps
	//FT_CharMap *faceCharacterMapArray = face->charmaps;    // charmap array
	
	//FT_Select_Charmap(face, encoding);     // select charmap with FT_Encoding handle (e.g. FT_ENCODING_UNICODE)
	//FT_Set_Charmap(face, charmap);         // select charmap with FT_CharMap handle (from face->charmaps array)
}

void FontManager::UnloadFontFace(const std::string &fontName)
{
	auto it = mFontFaces.find(fontName);

	if (it == mFontFaces.end())
		Logger::Instance().Log("Error: font face not found", Logger::Level::LOG_ERROR);

	FT_Done_Face(it->second);
	mFontFaces.erase(it);
}

#include <glew/GL/glew.h>

void FontManager::LoadFont(std::string const &fontName, unsigned pixelSize)
{
	FT_Face face = mFontFaces[fontName];

	//if (FT_Set_Char_Size(face, 0, height, hDPI, vDPI) != FT_Err_Ok)   // set fonts' width and height in fixed 26.6 point units - 0 width means same as height 
	//{
	//	LogError("can't set font size");
	//	Exit();
	//}

	if (FT_Set_Pixel_Sizes(face, 0, pixelSize) != FT_Err_Ok)   // set font's width and height in pixels - 0 width means same as height 
		Logger::Instance().Log("can't set font size", Logger::Level::LOG_ERROR);  // fixed/non-scalable font format (no available size)

	unsigned int lineSpacing = face->size->metrics.height >> 6;  // font face metrics 

	FT_GlyphSlot glyph = face->glyph;        // a glyph slot contains a glyph of its parent face (one at a time)
	
	unsigned int fontAtlasWidth = 0U, fontAtlasHeight = 0U;
	
	// compute texture atlas dimension 
	for (unsigned int i = 32; i < 128; i++) // i: ASCII code point/char code
	{
		//if (FT_Load_Char(face, i, FT_LOAD_DEFAULT) != FT_Err_Ok)  // load a character in the face's glyph slot and render the bitmap 
		//{
		//	LogError("can't load font character");
		//	Exit();
		//}

		//FT_Bitmap glyphBitmap = glyph->bitmap;  // rendered bitmap

		//fontAtlasWidth += glyphBitmap.width;

		//if (glyphBitmap.rows > fontAtlasHeight)
		//	fontAtlasHeight = glyphBitmap.rows;

		unsigned int glyphIndex = FT_Get_Char_Index(face, i);  // use UTF32 code point for i (default charmap is Unicode)

		if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT) != FT_Err_Ok)
			Logger::Instance().Log("can't load font character", Logger::Level::LOG_ERROR);

		FT_Glyph_Metrics glyphMetrics = glyph->metrics;  // glyph metrics

		fontAtlasWidth += glyphMetrics.width >> 6;

		if (glyphMetrics.height >> 6 > fontAtlasHeight)
			fontAtlasHeight = glyphMetrics.height >> 6;
	}

	Texture fontAtlas(fontAtlasWidth , fontAtlasHeight, 1U);
	
	Vector<Character> characters;
	
	int offset = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int i = 32; i < 128; i++)  // load texture atlas and characters
	{
		//unsigned int glyphIndex = FT_Get_Char_Index(face, i);  // use UTF32 code point for i (default charmap is Unicode)

		//if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT) != FT_Err_Ok)
		//{
		//	LogError("can't load font character");
		//	Exit();
		//}

		//FT_Load_Char(face, i, FT_LOAD_DEFAULT);               // load glyph with character code (skip glyph index)

		//FT_Load_Char(face, i, FT_LOAD_RENDER);                // load glyph from character code and render glyph to antialiased bitmap
		
		FT_Load_Char(face, i, FT_LOAD_DEFAULT);                 // load glyph from character code
		
		//int advanceX = glyph->advance.x >> 6;
		//int advanceY = glyph->advance.y >> 6;

		FT_Glyph_Metrics glyphMetrics = glyph->metrics;         // glyph metrics in 26.6 fixed point fractional pixel format - 1/64th of a pixel units. Use FT_LOAD_NO_SCALE when loading glyph to get metrics in font/point units 

		int width = glyphMetrics.width >> 6;                    // convert glyph metrics to integer pixel units
		int height = glyphMetrics.height >> 6;
		int bearingX = glyphMetrics.horiBearingX >> 6;
		int bearingY = glyphMetrics.horiBearingY >> 6;
		int advanceX = glyphMetrics.horiAdvance >> 6;
		int advanceY = glyphMetrics.vertAdvance >> 6;

		float textureOffsetX = (float)offset / (float)fontAtlasWidth;            // x offset in side texture atlas
		float textureOffsetY = 0.0f;     // y offset inside texture atlas
		float textureWidth = (float)width / (float)fontAtlasWidth;               // width in texture coordinates units
		float textureHeight = (float)height / (float)fontAtlasHeight;            // height in texture coordinates units

		characters.InsertLast(Character{ bearingX, bearingY, width, height, advanceX, advanceY, textureOffsetX, textureOffsetY, textureWidth, textureHeight });

		if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) != FT_Err_Ok)  // render glyph bitmap
			Logger::Instance().Log("can't render font character", Logger::Level::LOG_ERROR);

		// after bitmapping the glyph
		FT_Bitmap glyphBitmap = glyph->bitmap;  // glyph slot contains antialiased bitmap 
		int leftBearing = glyph->bitmap_left;   // character's left bearing (pen to left side of bounding box) in integer pixel units
		int topBearing = glyph->bitmap_top;     // character's top bearing (pen to top side of bounding box) in integer pixel units

		fontAtlas.Update(offset, 0, glyphBitmap.width, glyphBitmap.rows, glyphBitmap.buffer);  // update texture atlas with character data
		offset += glyphBitmap.width;
	}

	if (mFonts.find(fontName) == mFonts.end())
		mFonts.insert(std::pair<std::string,std::map<unsigned, Font>>(fontName, std::map<unsigned, Font>()));

	mFonts[fontName].insert(std::pair<unsigned ,Font>(pixelSize, Font(characters, std::move(fontAtlas), lineSpacing, fontName, pixelSize)));
}

