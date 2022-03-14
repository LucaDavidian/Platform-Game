#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <string>
#include <map>
#include "../Component.h"

class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(const std::string &textureName, unsigned int numRows = 1, unsigned int numColumns = 1, unsigned int zOrder = 0);

	void SetTexture(const std::string &textureName, unsigned int numRows = 1, unsigned int numColumns = 1);
	Texture *GetTexture() const { return mTextureAtlas; }

	unsigned int GetNumRows() const { return mNumRows; }
	unsigned int GetNumColumns() const { return mNumColumns; }
	
	void SetTile(unsigned int tile) { if (tile >= mNumRows * mNumColumns) return; mTile = tile; }
	unsigned int GetTile() const { return mTile; }

	unsigned int GetZOrder() const { return mZorder; }

	void Flip(bool flip) { mIsFlipped = flip; }
	bool IsFlipped() const { return mIsFlipped; }
private:
	Texture *mTextureAtlas;
	unsigned int mNumRows;
	unsigned int mNumColumns;

	unsigned int mZorder;

	unsigned int mTile = 0;   // current tile to be drawn

	bool mIsFlipped = false;

	static const bool mRegistered;
};

#endif // SPRITE_COMPONENT_H
