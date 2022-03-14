#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Texture.h"
#include <string>
#include <map>

class TextureManager
{
public:
	static TextureManager &GetInstance() { static TextureManager instance; return instance; }
	Texture &GetTexture(const std::string &textureName);
private:
	TextureManager() = default;
	std::map<std::string, Texture> mLoadedTextures;  // texture cache
};
#endif  // TEXTURE_MANAGER_H