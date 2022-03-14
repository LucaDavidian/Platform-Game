#ifndef FONT_H
#define FONT_H

#include "../data structures/vector.hpp"
#include "Texture.h"

struct Character
{
	int bearingX;
	int bearingY;
	int width;
	int height;
	int advanceX;
	int advanceY;
	float textureOffsetX;
	float textureOffsetY;
	float textureWidth;
	float textureHeight;
};

class Font
{
public:
	Font(const Vector<Character> &characters, Texture &&fontAtlas, unsigned lineSpacing, const std::string &name, unsigned int size) : mCharacters(characters), mFontAtlas(std::move(fontAtlas)), mLineSpacing(lineSpacing), mName(name), mSize(size) {}
	const Texture &GetAtlas() const { return mFontAtlas; }
	Character GetCharacter(char character) const { if (character < 32 || character >= 128) return mCharacters[63]; return mCharacters[character - 32]; }
	unsigned int GetLineSpacing() const { return mLineSpacing; }
	const std::string &GetName() const { return mName; }
	unsigned int GetSize() const { return mSize; }
private:
	Vector<Character> mCharacters;
	unsigned mLineSpacing;
	Texture mFontAtlas;
	std::string mName;
	unsigned int mSize;
};

#endif  // FONT_H